cmake_minimum_required(VERSION 2.4)
PROJECT(turglem-russian-examples)

SET(CMAKE_INSTALL_PREFIX /usr/local)

SET(CMAKE_CXX_FLAGS "-O3 -Wall -fomit-frame-pointer -pedantic -Wno-long-long -fPIC")
SET(CMAKE_C_FLAGS "-O3 -Wall -fomit-frame-pointer -pedantic -Wno-long-long -fPIC")

FIND_PATH(MAFSA_INCLUDE MAFSA/automaton.h PATHS /opt/include /usr/include /usr/local/include)
FIND_PATH(TURGLEM_INCLUDE turglem/lemmatizer.h PATHS /opt/include /usr/include /usr/local/include)
FIND_PATH(TL_RU_CHA_INCLUDE turglem/russian/charset_adapters.h PATHS /opt/include /usr/include /usr/local/include)

FIND_LIBRARY(TURGLEM_LIBRARY NAMES turglem PATHS /opt/lib /usr/lib /usr/local/lib)
FIND_LIBRARY(TURGLEM_RUSSIAN_LIBRARY NAMES turglem-russian PATHS /opt/lib /usr/lib /usr/local/lib)
FIND_LIBRARY(MAFSA_LIBRARY NAMES MAFSA PATHS /opt/lib /usr/lib /usr/local/lib)

INCLUDE_DIRECTORIES(${MAFSA_INCLUDE} ${TURGLEM_INCLUDE} ${TL_RU_CHA_INCLUDE})

ADD_EXECUTABLE(turglem-client turglem-client.c)
TARGET_LINK_LIBRARIES(turglem-client ${TURGLEM_LIBRARY} ${MAFSA_LIBRARY} ${TURGLEM_RUSSIAN_LIBRARY})

