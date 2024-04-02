#ifndef HIDELOOSEASSETSERVICE_H
#define HIDELOOSEASSETSERVICE_H

#include "bsapacker_global.h"
#include <bsapacker/IHideLooseAssetService.h>
#include <bsapacker/ISettingsService.h>

namespace BsaPacker
{
	class BSAPACKER_EXPORT HideLooseAssetService : public IHideLooseAssetService
	{
	public:
		HideLooseAssetService(const ISettingsService* settingsService);
		bool HideLooseAssets(const QDir& modDirectory) const override;
		void DeleteEmptyDirs(const QDir& dir) const;

		static QString s_HiddenExt;

	private:
		const ISettingsService* m_SettingsService = nullptr;
	};
}

#endif // HIDELOOSEASSETSERVICE_H
