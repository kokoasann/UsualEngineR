#pragma once




namespace UER
{
	extern int DebugConsoleUser;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="USER"></param>
	/// <param name="str"></param>
	static void DebugPrintLineConsole(int USER, const char* str)
	{
#if DEBUG_FUNC
		if (DebugConsoleUser & USER)
		{
			printf(str);
			printf("\n");
		}

#endif
	}

	template<class E>
	static void DebugPrintVector2(E user, const Vector2& v)
	{
#if DEBUG_FUNC

		char st[255] = { 0 };

		sprintf(st, "x: %f y: %f", v.x, v.y);
		DebugPrintLineConsole(TO_INT(user), st);

#endif
	}

	template<class E>
	static void DebugPrintVector3(E user, const Vector3& v)
	{
#if DEBUG_FUNC

		char st[255] = { 0 };

		sprintf(st, "x: %f y: %f z: %f", v.x, v.y, v.z);
		DebugPrintLineConsole(TO_INT(user), st);

#endif
	}

	template<class E>
	static void DebugPrintVector4(E user, const Vector4& v)
	{
#if DEBUG_FUNC

		char st[255] = { 0 };

		sprintf(st, "x: %f y: %f z: %f w: %f", v.x, v.y, v.z, v.w);
		DebugPrintLineConsole(TO_INT(user), st);

#endif
	}

	template<class E>
	static void DebugPrintMatrix(E user, const Matrix& v)
	{
#if DEBUG_FUNC

		char st[255] = { 0 };

		char c = 'x';
		for (int i = 0; i < 4; i++)
		{
			sprintf(st, "%cx: %f %cy: %f %cz: %f  %cw: %f", c, v.m[i][0], c, v.m[i][1], c, v.m[i][2], c, v.m[i][2], c, v.m[i][3]);
			DebugPrintLineConsole(TO_INT(user), st);
			c += 1;
			if (i == 2)
			{
				c = 'w';
			}
		}
#endif
	}

	template<class E,class T>
	static void DebugPrintValue(E user, const char* label, T value)
	{
#if DEBUG_FUNC

		std::string str = label;
		str += ": ";
		str += std::to_string(value);
		
		DebugPrintLineConsole(TO_INT(user), str.c_str());

#endif
	}
}