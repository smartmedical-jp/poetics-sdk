package poetics

import (
	"github.com/deepmap/oapi-codegen/pkg/securityprovider"
)

func NewRestClientWithResponses(apiKey string) (*ClientWithResponses, error) {
	apiKeyProvider, err := securityprovider.NewSecurityProviderApiKey("header", "X-API-Key", apiKey)
	if err != nil {
		return nil, err
	}
	return NewClientWithResponses(
		"https://rest.api.poetics-ai.com/v1",
		WithRequestEditorFn(apiKeyProvider.Intercept),
	)
}
