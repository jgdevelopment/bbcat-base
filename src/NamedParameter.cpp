
#define BBCDEBUG_LEVEL 1
#include "NamedParameter.h"

BBC_AUDIOTOOLBOX_START

#if ENABLE_JSON
namespace json
{
  /*--------------------------------------------------------------------------------*/
  /** Convert from list of NamedParameter objects to a JSON object
   *
   * @param list of NamedParameter objects
   * @param obj JSON object to be populated
   * @param all true to include parameters that are at their default
   */
  /*--------------------------------------------------------------------------------*/
  void ToJSON(const INamedParameter * const *list, uint_t n, JSONValue& obj, bool all)
  {
    uint_t i;

    // iterate through each parameter
    for (i = 0; i < n; i++)
    {
      const INamedParameter *parameter = list[i];
    
      // only add to object if parameter has been set or 'all' as been set
      if (all || parameter->IsSet()) parameter->ToJSON(obj[parameter->GetName()]);
    }
  }
  void ToJSON(const std::vector<INamedParameter *>& list, JSONValue& obj, bool all)
  {
    ToJSON(&list[0], list.size(), obj, all);
  }

  /*--------------------------------------------------------------------------------*/
  /** Convert from a JSON object to a set of NamedParameters
   *
   * @param obj JSON object to be as the source
   * @param list of NamedParameter objects to extract
   * @param reset true to reset parameters that are not specified
   *
   * @return true if all [found] parameters were evaluated properly
   */
  /*--------------------------------------------------------------------------------*/
  bool FromJSON(const JSONValue& obj, INamedParameter * const *list, uint_t n, bool reset)
  {
    uint_t i;
    bool success = true;

    // iterate through each parameter
    for (i = 0; i < n; i++)
    {
      INamedParameter *parameter = list[i];

      if (obj.isMember(parameter->GetName()))
      {      
        // if parameter exists, set it
        BBCDEBUG3(("Member '%s' found in '%s'", parameter->GetName(), ToJSONString(obj, true).c_str())); 
        success &= parameter->FromJSON(obj[parameter->GetName()]);
      }
      else if (reset)
      {
        // else reset parameter to its default
        BBCDEBUG2(("Member '%s' *not* found in '%s'", parameter->GetName(), ToJSONString(obj, true).c_str())); 
        parameter->Reset();
      }
    }

    return success;
  }
  bool FromJSON(const JSONValue& obj, const std::vector<INamedParameter *>& list, bool reset)
  {
    return FromJSON(obj, &list[0], list.size(), reset);
  }
};
#endif

BBC_AUDIOTOOLBOX_END
