#pragma once
#include <vector>
#include <memory>

template<typename ReturnType, typename ... ArgsType>
struct IDelegate {
	virtual ReturnType operator()(ArgsType... args) = 0;
};

//member function delegation
template<typename ClassType, typename ReturnType, typename ... ArgsType>
class DynamicDelegate : public IDelegate<ReturnType, ArgsType...> {
public:
	using FuncT = ReturnType(ClassType::*)(ArgsType...);

	DynamicDelegate() = delete;
	explicit DynamicDelegate(ClassType* classObjPtr, FuncT funcPtr)
		: _classObjPtr(classObjPtr), _funcPtr(funcPtr) {  }
	~DynamicDelegate() = default;

	ReturnType operator()(ArgsType... args) override {
		return (_classObjPtr->*_funcPtr)(args...);
	}

private:
	ClassType* _classObjPtr;
	FuncT		_funcPtr;
};

//normal function delegation
template<typename ReturnType, typename ... ArgsType>
class StaticDelegate : public IDelegate<ReturnType, ArgsType...> {
public:
	using FuncT = ReturnType(*)(ArgsType...);

	StaticDelegate() = delete;
	explicit StaticDelegate(FuncT funcPtr) : _funcPtr(funcPtr) {  }
	~StaticDelegate() = default;

	ReturnType operator()(ArgsType... args) override {
		return (*_funcPtr)(args...);
	}

private:
	FuncT _funcPtr{};
};

// single delegation
template<typename ReturnType, typename ... ArgsType>
class FunctionDelegate {
public:
	explicit FunctionDelegate() = default;

	// constructor for normal function
	explicit FunctionDelegate(typename StaticDelegate<ReturnType, ArgsType ...>::FuncT funcPtr) {
		Bind(funcPtr);
	}

	// constructor for member function
	template<typename ClassType>
	explicit FunctionDelegate(ClassType* classObjOPtr, typename DynamicDelegate<ClassType, ReturnType, ArgsType ...>::FuncT funcPtr) {
		Bind(classObjOPtr, funcPtr);
	}

	~FunctionDelegate() {
		Clear();
	}

	// bind normal function
	void Bind(typename StaticDelegate<ReturnType, ArgsType ...>::FuncT funcPtr) {
		Clear();
		_delegatePtr = std::make_unique<StaticDelegate<ReturnType, ArgsType...>>(funcPtr);
	}

	// bind member function
	template<typename ClassType>
	void Bind(ClassType* classObjPtr, typename DynamicDelegate<ClassType, ReturnType, ArgsType ...>::FuncT funcPtr) {
		Clear();
		_delegatePtr = std::make_unique<DynamicDelegate<ClassType, ReturnType, ArgsType ...>>(classObjPtr, funcPtr);
	}

	ReturnType Execute(ArgsType ... args) {
		if (_delegatePtr.get()) {
			return (*_delegatePtr)(args ...);
		}
		return ReturnType{};
	}

	void Clear() {
		_delegatePtr = nullptr;
	}

private:
	std::unique_ptr<IDelegate<ReturnType, ArgsType...>> _delegatePtr = nullptr;
};

// multi delegation
template<typename ... ArgsType>
class MultipleDelegate {
public:
	explicit MultipleDelegate() = default;
	~MultipleDelegate() = default;

	template<typename ClassType>
	void AddFunction(ClassType* classObjPtr, typename DynamicDelegate<ClassType, void, ArgsType ...>::FuncT funcPtr) {
		_delegatePtrs.push_back(std::make_unique<DynamicDelegate<ClassType, void, ArgsType ...>>(classObjPtr, funcPtr));
	}

	void AddFunction(typename StaticDelegate<void, ArgsType ...>::FuncT funcPtr) {
		_delegatePtrs.push_back(std::make_unique<StaticDelegate<void, ArgsType ...>>(funcPtr));
	}

	void Broadcast(ArgsType... args) {
		for (uint32_t i = 0; i < _delegatePtrs.size(); i++) {
			(*_delegatePtrs[i])(args ...);
		}
	}

	void Clear() {
		_delegatePtrs.Clear();
	}

private:
	std::vector<std::unique_ptr<IDelegate<void, ArgsType ...>>> _delegatePtrs;
};

// macros for using delegation easily
#ifndef DECLARE_FUNCTION_DELEGATE
#define DECLARE_FUNCTION_DELEGATE(DelegateName, ...) typedef FunctionDelegate<void, __VA_ARGS__> (DelegateName);
#endif

#ifndef DECLARE_FUNCTION_DELEGATE_WITH_RETURN_VALUE
#define DECLARE_FUNCTION_DELEGATE_WITH_RETURN_VALUE(ReturnType, DelegateName, ...) typedef FunctionDelegate<ReturnType, __VA_ARGS__> (DelegateName);
#endif

#ifndef DECLARE_MULTICAST_FUNCTION_DELEGATE
#define DECLARE_MULTICAST_FUNCTION_DELEGATE(DelegateName, ...) typedef MultipleDelegate<__VA_ARGS__> (DelegateName);
#endif
