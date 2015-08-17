cmake_minimum_required(VERSION 2.8)

# Generate configuration files for various test suites:
include(../cmake/configure-suite.cmake)

configure_file(veri.cfg veri.cfg)
configure_file(verify-cover.cfg verify-cover.cfg)
configure_file(cover.plain.cfg cover.plain.cfg)
configure_file(cover.encoded.cfg cover.encoded.cfg)
