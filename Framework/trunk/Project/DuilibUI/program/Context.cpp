#include "stdafx.h"
#include "Context.h"

Context::Context()
{
	m_programPath = Environment::GetExecutablePath(true);
	m_programDir = Path::GetDirectoryName(m_programPath);
}
