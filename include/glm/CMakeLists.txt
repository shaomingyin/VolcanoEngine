file(GLOB ROOT_INLINE *.inl)
file(GLOB ROOT_HEADER *.hpp)

file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ./detail/*.hpp)

file(GLOB_RECURSE EXT_INLINE ./ext/*.inl)
file(GLOB_RECURSE EXT_HEADER ./ext/*.hpp)

file(GLOB_RECURSE GTC_INLINE ./gtc/*.inl)
file(GLOB_RECURSE GTC_HEADER ./gtc/*.hpp)

file(GLOB_RECURSE GTX_INLINE ./gtx/*.inl)
file(GLOB_RECURSE GTX_HEADER ./gtx/*.hpp)

file(GLOB_RECURSE SIMD_INLINE ./simd/*.inl)
file(GLOB_RECURSE SIMD_HEADER ./simd/*.h)

target_sources(Volcano
	PUBLIC
		${ROOT_INLINE}    ${ROOT_HEADER}
		${CORE_INLINE}    ${CORE_HEADER}
		${EXT_INLINE}     ${EXT_HEADER}
		${GTC_INLINE}     ${GTC_HEADER}
		${GTX_INLINE}     ${GTX_HEADER}
		${SIMD_INLINE}    ${SIMD_HEADER}
	)
