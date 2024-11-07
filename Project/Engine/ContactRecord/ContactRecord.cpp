#include "ContactRecord.h"
#include <algorithm>

void ContactRecord::AddHistory(uint32_t number)
{

	history_.push_back(number);

}

bool ContactRecord::ConfirmHistory(uint32_t number)
{

	return std::any_of(history_.begin(), history_.end(), [number](uint32_t x) { return x == number; });

}

void ContactRecord::Clear()
{

	history_.clear();

}
