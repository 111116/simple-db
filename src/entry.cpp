#include "entry.h"

/**
 * Updates this entry with values specified.
 * 
 * @param A vector of column-value pairs indicating how to modify this entry
*/
void Entry::set(set_t values)
{
	for (update_t& u: values)
		data[u.first] = u.second;
}
