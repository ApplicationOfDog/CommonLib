#pragma once
#include <string>

#ifdef _UNICODE
struct SignerCertificate
{
	std::wstring IssuerName;
	std::wstring SubjectName;
};
#else
struct SignerCertificate
{
	std::string IssuerName;
	std::string SubjectName;
};
#endif

int GetSignatureInfo(std::wstring path, SignerCertificate &SignerInfo);
