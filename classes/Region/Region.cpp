#include "Region.h"
#include "../DBFieldLength/DBFieldLength.h"
#include "../ErrorList/ErrorList.h"
#include "../../functions/split.h"

std::vector<Region*> Region::allregions;
std::unordered_map<std::string, Region*> Region::code_hash;

Region::Region (const std::string &line, ErrorList &el)
{	active_only_mileage = 0;
	active_preview_mileage = 0;
	overall_mileage = 0;
	// parse CSV line
	size_t NumFields = 5;
	std::string country_str, continent_str;
	std::string* fields[5] = {&code, &name, &country_str, &continent_str, &type};
	split(line, fields, NumFields, ';');
	if (NumFields != 5)
	{	el.add_error("Could not parse regions.csv line: [" + line + "], expected 5 fields, found " + std::to_string(NumFields));
		is_valid = 0;
		return;
	}
	is_valid = 1;
	// code
	if (code.size() > DBFieldLength::regionCode)
		el.add_error("Region code > " + std::to_string(DBFieldLength::regionCode)
			   + " bytes in regions.csv line " + line);
	// name
	if (name.size() > DBFieldLength::regionName)
		el.add_error("Region name > " + std::to_string(DBFieldLength::regionName)
			   + " bytes in regions.csv line " + line);
	// regionType
	if (type.size() > DBFieldLength::regiontype)
		el.add_error("Region type > " + std::to_string(DBFieldLength::regiontype)
			   + " bytes in regions.csv line " + line);
}//*/
