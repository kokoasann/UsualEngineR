/*!
 * @brief	行列。
 */

#include "PreCompile.h"
#include "Matrix.h"


namespace UER
{
	
	const Matrix Matrix::Identity(
			1.0f, 0.0f, 0.0f, 0.0f ,
			0.0f, 1.0f, 0.0f, 0.0f ,
			0.0f, 0.0f, 1.0f, 0.0f ,
			0.0f, 0.0f, 0.0f, 1.0f
	); 

}
