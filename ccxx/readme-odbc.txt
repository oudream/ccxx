// otl的函数异常已关闭，用以下方法：
//   修改 otlv4.h 中代码#define OTL_ANSI_CPP_11_NOEXCEPT_FALSE noexcept(false) 改成 #define OTL_ANSI_CPP_11_NOEXCEPT_FALSE noexcept(true)
#define OTL_DESTRUCTORS_DO_NOT_THROW
//#define OTL_ANSI_CPP_11_NOEXCEPT_FALSE noexcept(true)

// The following #define is required with MyODBC 3.51.11 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
//#define OTL_UNCAUGHT_EXCEPTION_OFF 关异常
