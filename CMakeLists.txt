set(COMPONENT_SRCS
    "src/gnss_parser.cpp"
    "src/Private/gnss_parser_priv.cpp"
    "src/Parser/gnss_parser_gga.cpp"
    "src/Parser/gnss_parser_gsa.cpp"
    "src/Parser/gnss_parser_gsv.cpp"
    "src/Parser/gnss_parser_vtg.cpp"
    "src/Parser/gnss_parser_gll.cpp"
    "src/Parser/gnss_parser_rmc.cpp"
    )

set(COMPONENT_ADD_INCLUDEDIRS
	"include"
	)

set(COMPONENT_PRIV_REQUIRES)

register_component()