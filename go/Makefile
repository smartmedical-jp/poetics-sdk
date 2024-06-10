init:
	go mod tidy

codegen:
	go install github.com/deepmap/oapi-codegen/v2/cmd/oapi-codegen@latest
	oapi-codegen --generate client,types --package rest ../rest.spec.json  > rest/rest_client.gen.go

test:
ifndef POETICS_API_KEY
	$(error POETICS_API_KEY is not set)
endif
	go test ./...
	@echo "All tests passed"