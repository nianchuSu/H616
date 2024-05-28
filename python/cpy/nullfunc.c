//	添加头文件Python.h,使用python函数
//	初始化python解释器,Py_Initialize()
//	需要用到的py文件路径导入到sys.path中,模块即文件(sys.path)搜索解释器路径
//	导入模块所在路径添加到path中
//	3.导入需要模块,传入参数--模块名
//	4.判断是否导入成功
//	5.获取模块中的函数的引用
//	6.判断是否获取函数引用是否成功
//
//	有参函数创建一个py元组没有参数就不需要
//
//	7.调用函数引用,并传入参数(有参函数需要再此之前生成参数元组)无参就传NULL
//	8.判断是否调用成功
//	9.释放函数返回值的对象
//	10.释放函数引用
//	11.释放模块引用
//	12.py解释器的关闭
#include "Python.h"

/*
 *调用无参数无返回值函数
*/
int main()
{
	Py_Initialize();	// 初始化解释器
						//
	PyObject *sys = PyImport_ImportModule("sys");		// 当前路径或者需要用的py模块/文件路径导入
	PyObject *path = PyObject_GetAttrString(sys,"path");	// sys导入path列表	获取path引用
	PyList_Append(path, PyUnicode_FromString("."));	// 将当前路径添加到path列表里	将c字符串转化为py字符串
	PyObject *pModule = PyImport_ImportModule("cpy");	// 导入模块,获取对应模块的引用,参数就是使用的模块名
	
	if(!pModule){	// 为NULL就进入
		PyErr_Print();	// 调用py异常报错
		printf("error Module\n");
		exit(-1);
	}

	PyObject *pFunc = PyObject_GetAttrString(pModule,"pr");	// 获取模块中已有的函数名称如cpy模块中的pr函数的对象与引用,并传入C字符串参数函数名
	if(!pFunc){	// 为NULL就进入
		PyErr_Print();	// 调用py异常报错
		printf("error Func\n");
		exit(-1);
	}


	PyObject *pValue = PyObject_CallObject(pFunc,NULL);	// 调用函数获取返回值,参数1函数引用参数2是函数参数元组
	if(!pValue){	// 为NULL就进入
		PyErr_Print();	// 调用py异常报错
		printf("error Value\n");
		exit(-1);
	}

	Py_DECREF(pValue);	// 关闭所有对象,依次向上关闭
	Py_DECREF(pFunc);	// 关闭所有对象,依次向上关闭
	Py_DECREF(pModule);	// 关闭所有对象,依次向上关闭

	Py_Finalize();	// py解释器关闭
	return 0;
}
