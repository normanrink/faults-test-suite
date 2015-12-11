cmake_minimum_required(VERSION 2.8)


configure_file(expr.encoded.veri.cfg expr.encoded.veri.cfg)

configure_file(expr.lex.encoded.cover.cfg expr.lex.encoded.cover.cfg)
configure_file(expr.lex.plain.cover.cfg expr.lex.plain.cover.cfg)

configure_file(expr.parse.encoded.cover.cfg expr.parse.encoded.cover.cfg)
configure_file(expr.parse.plain.cover.cfg expr.parse.plain.cover.cfg)

configure_file(expr.eval.encoded.cover.cfg expr.eval.encoded.cover.cfg)
configure_file(expr.eval.plain.cover.cfg expr.eval.plain.cover.cfg)
