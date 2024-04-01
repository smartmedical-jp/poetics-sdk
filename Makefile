test:
ifndef POETICS_API_KEY
	$(error POETICS_API_KEY is not set)
endif
	go test -count=1 -v ./...

codegen:
	oapi-codegen --generate client,types --package poetics rest.spec.json  > poetics/rest_client.gen.go

download_spec_from_readme:
	curl -o rest.spec.json https://poetics-api.readme.io/openapi/643c9ee349045100346456d3