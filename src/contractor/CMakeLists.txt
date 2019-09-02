# see arithmetic/CMakeLists.txt for comments

list (APPEND IBEX_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ContractContext.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Ctc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Ctc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Ctc3BCid.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Ctc3BCid.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcAcid.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcAcid.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcCompo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcCompo.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcEmpty.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcEmpty.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcExist.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcExist.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcFixPoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcFixPoint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcForAll.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcForAll.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcFwdBwd.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcFwdBwd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcHC4.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcHC4.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcIdentity.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcIdentity.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcInteger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcInteger.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcInverse.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcInverse.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcKhunTucker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcKhunTucker.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcLinearRelax.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcLinearRelax.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcNewton.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcNewton.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcNotIn.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcNotIn.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcPolytopeHull.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcPolytopeHull.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcPropag.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcPropag.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcQInter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcQInter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcQuantif.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcQuantif.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcUnion.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcUnion.h
  )

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Propagate new values of list to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
