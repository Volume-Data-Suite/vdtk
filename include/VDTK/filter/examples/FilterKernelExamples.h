
#include "../../common/Common.h"


namespace VDTK::FilterKernelExamples {

	static const FilterKernel filterAverage3x3x3(3,
		{
		{ {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0}, {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0}, {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0} },
		{ {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0}, {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0}, {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0} },
		{ {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0}, {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0}, {1.0 / 27.0, 1.0 / 27.0, 1.0 / 27.0} },
		}
	);

	static const FilterKernel filterAverage5x5x5(5,
		{
		{ {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0} },
		{ {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0} },
		{ {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0} },
		{ {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0} },
		{ {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0}, {1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0, 1.0 / 125.0} },
		}
	);

	static const FilterKernel filterSmooth3x3x3(3,
		{
		{ {0.0 / 24.0, 0.0 / 24.0, 0.0 / 24.0}, {1.0 / 24.0, 2.0 / 24.0, 1.0 / 24.0}, {0.0 / 24.0, 0.0 / 24.0, 0.0 / 24.0} },
		{ {1.0 / 24.0, 2.0 / 24.0, 1.0 / 24.0}, {2.0 / 24.0, 4.0 / 24.0, 2.0 / 24.0}, {1.0 / 24.0, 2.0 / 24.0, 1.0 / 24.0} },
		{ {0.0 / 24.0, 0.0 / 24.0, 0.0 / 24.0}, {1.0 / 24.0, 2.0 / 24.0, 1.0 / 24.0}, {0.0 / 24.0, 0.0 / 24.0, 0.0 / 24.0} },
		}
	);

	static const FilterKernel filterExpertimentalHighPass3x3x3(3,
		{
		{ {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0}, {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0}, {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0} },
		{ {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0}, {-1.0 / 3.0, 9.0, -1.0 / 3.0}, {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0} },
		{ {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0}, {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0}, {-1.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0} },
		}
	);
}