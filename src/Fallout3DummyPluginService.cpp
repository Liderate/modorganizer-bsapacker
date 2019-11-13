#include "Fallout3DummyPluginService.h"

namespace BsaPacker
{
	Fallout3DummyPluginService::Fallout3DummyPluginService(const IFileWriterService* fileWriterService, const IDummyPluginLogic* dummyPluginLogic)
		: m_FileWriterService(fileWriterService), m_DummyPluginLogic(dummyPluginLogic)
	{
	}

	bool Fallout3DummyPluginService::CreatePlugin(const QString& modPath, const QString& archiveNameBase) const
	{
		const QString& fileNameNoExtension = modPath + '/' + archiveNameBase;
		if (!this->m_DummyPluginLogic->canCreateDummyESP(fileNameNoExtension))
		{
			return false;
		}
		const std::string& absoluteFileName = fileNameNoExtension.toStdString() + ".esp";
		return this->m_FileWriterService->Write(absoluteFileName,
												reinterpret_cast<const char*>(this->RAW_FALLOUT3),
												this->RAW_FO3_SIZE);
	}
}
