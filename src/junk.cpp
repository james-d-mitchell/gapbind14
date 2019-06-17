
// Version 1
//  static void testing1() {
//   std::cout << "it works" << std::endl;
// }

// Version 2
// static std::string testing1() {
//  return "it works";
// }

// Version 3
// static void testing1(std::string str) {
//  std::cout << "it works " + str;
// }

// Version 4
 static std::string testing1(std::string str) {
   return "it works " + str;
 }

template <typename TSignature, typename = void> struct wrapper {};

template <typename TReturnType>
struct wrapper<TReturnType()> {
  TReturnType operator()(TReturnType (*f)()) {
    return f();
  }
};

template <typename TReturnType, typename... TArgs>
struct wrapper<TReturnType(TArgs...),
               std::enable_if_t<sizeof...(TArgs) != 0>> {
  TReturnType operator()(TReturnType (*f)(TArgs...), TArgs... args) {
    return f(args...);
  }
  void operator()(TReturnType (*)(TArgs...)) {
  }
};

// static_assert(return_void(testing1), "See nothing");

template <typename TToGap, typename TToCpp, typename TDummy = Obj>
auto myfunc1(Obj self) -> std::enable_if_t<
    std::is_same<TToGap, gapbind14::to_gap_void>::value
        && std::is_same<TToCpp, gapbind14::to_cpp_void>::value,
    TDummy> {
  wrapper<decltype(testing1)>()(&testing1);
  return 0L;
}

template <typename TToGap, typename TToCpp, typename TDummy = Obj>
auto myfunc1(Obj self) -> std::enable_if_t<
    !std::is_same<TToGap, gapbind14::to_gap_void>::value
        && std::is_same<TToCpp, gapbind14::to_cpp_void>::value,
    TDummy> {
  return TToGap()(wrapper<decltype(testing1)>()(&testing1));
}

template <typename TToGap, typename TToCpp, typename TDummy = Obj>
auto myfunc1(Obj self, Obj arg1) -> std::enable_if_t<
    std::is_same<TToGap, gapbind14::to_gap_void>::value
        && !std::is_same<TToCpp, gapbind14::to_cpp_void>::value,
    TDummy> {
  wrapper<decltype(testing1)>()(&testing1, TToCpp()(arg1));
  return 0L;
}

template <typename TToGap, typename TToCpp, typename TDummy = Obj>
auto myfunc1(Obj self, Obj arg1) -> std::enable_if_t<
    !std::is_same<TToGap, gapbind14::to_gap_void>::value
        && !std::is_same<TToCpp, gapbind14::to_cpp_void>::value,
    TDummy> {
  return TToGap()(wrapper<decltype(testing1)>()(&testing1, TToCpp()(arg1)));
}
      // static_assert(detail::expected_num_args<Extra...>(sizeof...(Args),
      // cast_in::has_args, cast_in::has_kwargs),
      //            "The number of argument annotations does not match the
      //            number of function arguments");

      /* Stash some additional information used by an important optimization in
       * 'functional.h' */
      // using FunctionType = Return (*)(Args...);
      // constexpr bool is_function_ptr
      //    = std::is_convertible<Func, FunctionType>::value
      //      && sizeof(capture) == sizeof(void*);
template <typename TArgs, typename = void>
struct function_wrapper {};

template <typename TArgs>
struct function_wrapper<
    TArgs,
    std::enable_if_t<
        std::conditional_t<decltype(gapbind14::func_arg_count(testing))::value
                               == 0,
                           std::true_type,
                           std::false_type>::value,
        TArgs>> {
  void operator()() {
    testing();
  }
};

template <typename TDummy = Obj>
auto myfunc(Obj) -> std::enable_if_t<
    std::conditional_t<decltype(gapbind14::func_arg_count(testing))::value == 0,
                       std::true_type,
                       std::false_type>::value,
    TDummy> {
  function_wrapper<void>()();
  return 0L;
}

template <typename TArgs>
struct function_wrapper<
    TArgs,
    std::enable_if_t<
        std::conditional_t<decltype(gapbind14::func_arg_count(testing))::value
                               == 1,
                           std::true_type,
                           std::false_type>::value,
        TArgs>> {
  void operator()(TArgs arg) {
    testing(arg);
  }
};

template <typename TDummy = Obj>
auto myfunc(Obj) -> std::enable_if_t<
    std::conditional_t<decltype(gapbind14::func_arg_count(testing))::value == 1,
                       std::true_type,
                       std::false_type>::value,
    TDummy> {
  function_wrapper<TDummy>()(1);
  return 0L;
}


struct test_struct {
  void operator()() {
    std::cout << "struct: it works\n";
  }
};

template <typename TDummy = Obj>
auto myfunc2(Obj)
    -> std::enable_if_t<std::conditional_t<decltype(gapbind14::meth_arg_count(
                                               &test_struct::operator()))::value
                                               == 0,
                                           std::true_type,
                                           std::false_type>::value,
                        TDummy> {
  test_struct()();
  return 0L;
}

template <typename TDummy = Obj>                                            \
  auto GAPBIND14_CONCAT3(class_name, _, method)(Obj self, Obj arg1, Obj arg2) \
      ->std::enable_if_t<                                                     \
          std::conditional_t<decltype(gapbind14::meth_arg_count(              \
                                 &class_name::method))::value                 \
                                 == 1,                                        \
                             std::true_type,                                  \
                             std::false_type>::value,                         \
          TDummy> {                                                           \
    if (TNUM_OBJ(arg1) != gapbind14::T_GAPBIND14_OBJ) {                       \
      ErrorQuit(                                                              \
          "expected TGapBind14Obj but got %s!", (Int) TNAM_OBJ(arg1), 0L);    \
    }                                                                         \
    if (module.subtype(arg1)                                                  \
        != module.subtype(GAPBIND14_TO_STRING(class_name))) {                 \
      ErrorQuit("expected %s but got %s!",                                    \
                (Int) GAPBIND14_TO_STRING(class_name),                        \
                (Int) module.name(arg1));                                     \
    }                                                                         \
    class_name* ptr                                                           \
        = gapbind14::SubTypeSpec<class_name, TToGap, TToCpp>::obj_cpp_ptr(    \
            arg1);                                                            \
    try {                                                                     \
    } catch (std::exception const& e) {                                       \
      ErrorQuit(e.what(), 0L, 0L);                                            \
    }                                                                         \
    return 0L;                                                                \
  }                                                                           \
