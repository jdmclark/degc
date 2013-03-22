#include <nullunit/nullunit.h>
#include "runtime/solver/linearrecordtable.h"

using namespace Deg::Runtime::Math;
using namespace Deg::Runtime::Solver;

BeginSuite(LinearRecordTableTest);

Case(Construction) {
	LinearRecordTable t(3, 0);
	t.AddRecord({ DefaultFixed(1), DefaultFixed(2), DefaultFixed(3) });
}

Case(SimpleExists) {
	LinearRecordTable t(3, 0);
	t.AddRecord({ DefaultFixed(0), DefaultFixed(0), DefaultFixed(0) });
	t.AddRecord({ DefaultFixed(1), DefaultFixed(1), DefaultFixed(1) });
	t.AddRecord({ DefaultFixed(2), DefaultFixed(0), DefaultFixed(0) });

	Test_Assert(!t.IsEmpty(Set(3, 0, Relation::Equal, DefaultFixed(0))));
	Test_Assert(!t.IsEmpty(Set(3, 0, Relation::Equal, DefaultFixed(1))));
	Test_Assert(t.IsEmpty(Set(3, 2, Relation::Equal, DefaultFixed(2))));
}

Case(SimpleQuantity) {
	LinearRecordTable t(3, 0);
	t.AddRecord({ DefaultFixed(6), DefaultFixed(0), DefaultFixed(1) });
	t.AddRecord({ DefaultFixed(1), DefaultFixed(1), DefaultFixed(1) });
	t.AddRecord({ DefaultFixed(2), DefaultFixed(0), DefaultFixed(0) });

	Test_Assert_Eq(t.QuantityOf(Set(3, 1, Relation::Equal, DefaultFixed(0))), DefaultFixed(8));
	Test_Assert_Eq(t.QuantityOf(Set(3, 2, Relation::Equal, DefaultFixed(1))), DefaultFixed(7));
	Test_Assert_Eq(t.QuantityOf(Set(3, 1, Relation::Equal, DefaultFixed(0)) & Set(3, 2, Relation::Equal, DefaultFixed(1))), DefaultFixed(6));
}

EndSuite(LinearRecordTableTest);
