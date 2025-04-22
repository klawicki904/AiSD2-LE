#include "pch.h"
#include "CppUnitTest.h"
#include "../source CPP/Matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace FordFulkersonTests
{
	TEST_CLASS(MaxFlowClassicGraph)
	{
	public:
		
		TEST_METHOD(SimpleGraph1)
		{
			Matrix m(4);
			m.addEdge(0, 1, 10);
			m.addEdge(0, 2, 10);
			m.addEdge(1, 3, 10);
			m.addEdge(2, 3, 10);

			double result = m.fordFulkerson(0, 3);  

			Assert::AreEqual(20.0, result); 
		}
	};
}
