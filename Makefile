LANGS := go

init:
	@$(foreach lang,$(LANGS),$(MAKE) -C $(lang) init;)

codegen:
	@$(foreach lang,$(LANGS),$(MAKE) -C $(lang) codegen;)

test:
	@$(foreach lang,$(LANGS),$(MAKE) -C $(lang) test;)

download_spec_from_readme:
	curl -o rest.spec.json https://poetics-api.readme.io/openapi/643c9ee349045100346456d3

