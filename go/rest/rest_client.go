package rest

import (
	"os"

	"github.com/deepmap/oapi-codegen/pkg/securityprovider"
)

type RestClientOptions struct {
	ApiKey   string
	EndPoint string
}

func (o RestClientOptions) fallback() RestClientOptions {
	out := o
	if out.EndPoint == "" {
		out.EndPoint = os.Getenv("POETICS_REST_API_ENDPOINT")
	}
	if out.EndPoint == "" {
		out.EndPoint = "https://rest.api.poetics-ai.com/v1"
	}
	if out.ApiKey == "" {
		out.ApiKey = os.Getenv("POETICS_API_KEY")
	}
	if out.ApiKey == "" {
		panic("API key is required")
	}

	return out
}

func NewRestClient() (*ClientWithResponses, error) {
	return NewRestClientWithOptions(RestClientOptions{})
}

func NewRestClientWithOptions(options RestClientOptions) (*ClientWithResponses, error) {
	opts := options.fallback()
	apiKeyProvider, err := securityprovider.NewSecurityProviderApiKey("header", "X-API-Key", opts.ApiKey)
	if err != nil {
		return nil, err
	}
	return NewClientWithResponses(
		opts.EndPoint,
		WithRequestEditorFn(apiKeyProvider.Intercept),
	)
}
