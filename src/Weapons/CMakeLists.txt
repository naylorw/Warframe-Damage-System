add_subdirectory(Incarnon)
add_subdirectory(FiringModeImpl)

target_sources(${TARGET_NAME}
PRIVATE
DamageType.cpp
StatusEffect.cpp
ProcType.cpp
Weapon.cpp
WeaponData.cpp
WeaponFactory.cpp
FiringMode.cpp
)

set(CPP_SOURCES
${CPP_SOURCES}
${CMAKE_CURRENT_LIST_DIR}/Weapon.cpp
${CMAKE_CURRENT_LIST_DIR}/WeaponData.cpp
${CMAKE_CURRENT_LIST_DIR}/DamageType.cpp
${CMAKE_CURRENT_LIST_DIR}/StatusEffect.cpp
${CMAKE_CURRENT_LIST_DIR}/ProcType.cpp
${CMAKE_CURRENT_LIST_DIR}/WeaponFactory.cpp
${CMAKE_CURRENT_LIST_DIR}/FiringMode.cpp
PARENT_SCOPE)