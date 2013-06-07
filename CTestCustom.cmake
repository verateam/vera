set(CTEST_CUSTOM_COVERAGE_EXCLUDE
  ${CTEST_CUSTOM_COVERAGE_EXCLUDE}
  # exclude cpptcl - this is an external lib used by vera++
  "/cpptcl"
)
