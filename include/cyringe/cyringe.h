#pragma once

#include <unordered_map>
#include <mutex>

/** cyringe
 *  Dependency injection framework for C++
 *  cyringe allows to distribute/provide objects within the process borders.
 *
 *  The cyringe framework can provide single-instance of one type or multiple-instances of a type
 *  * In case of a single-instance, which is singleton-compatible, is no need to define the ID-Type and
 *    also no need to set the argument id when calling the cyringe-methods.
 *  * In case of a multi-instance, a proper ID-Type has to be defined (must be an integral-type or enum-type)
 *    and an unique ID per object has to be passed with every method call.
 *
 *  @tparam Tp_ The first template typename is the type of the instance/object
 *  @tparam Id_ The second template typename is the id-type of the instance/object in case multiple-instances, and it's by default std::int32_t */
template<typename Tp_, typename Id_ = std::int32_t>
class cyringe
{
  static_assert(std::is_integral<Id_>::value || std::is_enum<Id_>::value,
    "id must be an integral-type or an enum-type");

public:
  /** Object setup/registration
   *  The object to register is identified by the Type and
   *  in case of multiple instances also by the passed unique ID value.
   *
   *  Make sure the ID is unique per Type.
   *
   *  Do not destroy the instance until the instance is unregistered (call release() first).
   *
   *  \param object Reference to object
   *  \param id Global object id, in case multiple instances of the same Type */
  static void setup(Tp_& object, const Id_ id = 0)
  {
    auto& manager_ = manager();
    std::lock_guard<std::mutex> guard(manager_.mutex_);

    auto search_iterator = manager_.objects_.find(id);
    if (search_iterator != manager_.objects_.end())
    {
      throw std::runtime_error("this object is already registred, unregister first");
    }

    manager_.objects_.emplace(id, &object);
  }

  /** Get the reference to the object.
   *  Make sure the object is registered before usage.
   *  \param id Global object id, in case multiple instances of the same Type.
   *  \return Returns the reference to the requested object. */
  static Tp_& get(const Id_ id = 0)
  {
    auto& manager_ = manager();
    std::lock_guard<std::mutex> guard(manager_.mutex_);
    try
    {
      return *manager_.objects_.at(id);
    } catch (std::out_of_range) {
      throw std::runtime_error("requested object not yet registred");
    }
  }

  /** Release the Object.
   *  Afterwards the instance can be destroyed.
   *  The object is identified by the Type and the passed unique ID.
   *  \param id Global object id, in case multiple instances of the same Type */
  static void release(const Id_ id = 0)
  {
    auto& manager_ = manager();
    std::lock_guard<std::mutex> guard(manager_.mutex_);
    manager_.objects_.erase(id);
  }

private:
  /** Get the right manager for the passed template.
   *  This method creates a manager per template type. If the manager
   *  for a certain template type has already been created, then the right object-reference will be passed back.
   *  \return Returns reference to a new or existing manager */
  static cyringe<Tp_, Id_>& manager()
  {
    static cyringe<Tp_, Id_> manager_;
    return manager_;
  }

private:
  std::unordered_map<Id_, Tp_*> objects_;
  std::mutex mutex_;
};
