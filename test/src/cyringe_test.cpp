#include "cyringe/cyringe.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class TestClass {
};

TEST(test_cyringe, when_not_setup_then_get_throws_the_right_exception)
{
  EXPECT_THROW(
        {
          try {
            cyringe<TestClass>::get();
          }  catch (const std::runtime_error& e) {
            EXPECT_STREQ("requested object not yet registred", e.what());
            throw;
          }
        }, std::runtime_error);
}

TEST(test_cyringe, test_if_the_type_is_a_reference_to_the_given_type)
{
  //setup
  TestClass a;
  cyringe<TestClass>::setup(a);

  // exercise
  static_assert (std::is_same<TestClass&, decltype(cyringe<TestClass>::get())>::value, "");

  // tear down
  cyringe<TestClass>::release();
}

TEST(test_cyringe, test_if_the_pointer_to_instace_is_the_same)
{
  //setup
  TestClass a;
  cyringe<TestClass>::setup(a);

  // exercise
  ASSERT_TRUE(&a == &cyringe<TestClass>::get());

  // tear down
  cyringe<TestClass>::release();
}

TEST(test_cyringe, test_if_the_pointers_to_instaces_are_the_same)
{
  //setup
  TestClass a;
  TestClass b;
  TestClass c;
  cyringe<TestClass>::setup(a, 0);
  cyringe<TestClass>::setup(b, 1);
  cyringe<TestClass>::setup(c, 2);

  // exercise
  ASSERT_TRUE(&a == &cyringe<TestClass>::get(0));
  ASSERT_TRUE(&b == &cyringe<TestClass>::get(1));
  ASSERT_TRUE(&c == &cyringe<TestClass>::get(2));

  // tear down
  cyringe<TestClass>::release(0);
  cyringe<TestClass>::release(1);
  cyringe<TestClass>::release(2);
}

TEST(test_cyringe, test_use_after_release)
{
  //setup
  TestClass a;
  cyringe<TestClass>::setup(a);
  cyringe<TestClass>::release();

  // exercise
  EXPECT_THROW(
        {
          try {
            cyringe<TestClass>::get();
          }  catch (const std::runtime_error& e) {
            EXPECT_STREQ("requested object not yet registred", e.what());
            throw;
          }
        }, std::runtime_error);
}

TEST(test_cyringe, test_setup_multiple_times)
{
  //setup
  TestClass a;
  cyringe<TestClass>::setup(a);

  // exercise
  EXPECT_THROW(
        {
          try {
            cyringe<TestClass>::setup(a);
          }  catch (const std::runtime_error& e) {
            EXPECT_STREQ("this object is already registred, unregister first", e.what());
            // tear down
            cyringe<TestClass>::release();
            // rethrow
            throw;
          }
        }, std::runtime_error);
}

TEST(test_cyringe, test_release_multiple_times)
{
  // setup
  TestClass a;
  cyringe<TestClass>::setup(a);
  cyringe<TestClass>::release();

  // exercise
  cyringe<TestClass>::release();
}

class XYZ {
  int a;
  int b;
};

TEST(test_cyringe, test_setup_heap_instance){
  // setup
  XYZ* xyz = new XYZ();

  // exercise
  cyringe<XYZ>::setup(*xyz);

  // tear down
  cyringe<XYZ>::release();
  delete xyz;
  xyz = nullptr;
}
