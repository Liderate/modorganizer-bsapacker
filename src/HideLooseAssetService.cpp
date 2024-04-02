#include "HideLooseAssetService.h"

#include <QDir>
#include <QDirIterator>
#include <QString>
#include <QtConcurrent/QtConcurrentMap>

#include "SettingsService.h"

namespace BsaPacker
{
	QString HideLooseAssetService::s_HiddenExt(".mohidden");

	HideLooseAssetService::HideLooseAssetService(const ISettingsService* settingsService)
		: m_SettingsService(settingsService)
	{
	}

	bool HideLooseAssetService::HideLooseAssets(const QDir& modDirectory) const
	{
		if (!this->m_SettingsService->GetPluginSetting(SettingsService::SETTING_HIDE_LOOSE_ASSETS).toBool()) {
			return false;
		}

		// Only hide loose files that were not excluded when creating the archive (blacklisted), so might still be needed by the mod
		QStringList blacklistExtensions = this->m_SettingsService->GetPluginSetting(SettingsService::SETTING_BLACKLISTED_FILES).toString().split(';');
		for (auto& ext : blacklistExtensions) {
			ext.prepend("*");
		}

		const QString& absModDir = modDirectory.absolutePath();
		const QStringList& subDirs = modDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		QtConcurrent::blockingMap(subDirs, [&](const QString& subDir) {
			// Hide subdirectories
			const QString& absPath = absModDir + '/' + subDir;
			QDir originalDir(absPath);
			if (originalDir.dirName().endsWith(s_HiddenExt) || originalDir.isEmpty()) {
				return;
			}
			if (!originalDir.rename(originalDir.absolutePath(), originalDir.absolutePath() + s_HiddenExt)) {
				qWarning() << "Failed to hide " << originalDir.absolutePath();
				return;
			}

			// Restore files with blacklisted extension to their original directories
			QDir hiddenDir(originalDir.absolutePath() + s_HiddenExt);
			QDirIterator iterator(hiddenDir.absolutePath(), blacklistExtensions, QDir::Files, QDirIterator::Subdirectories);
			while (iterator.hasNext()) {
				QString hiddenFilePath = iterator.next();
				QString originalFilePath = originalDir.absoluteFilePath(hiddenDir.relativeFilePath(hiddenFilePath));
				originalDir.mkpath(originalFilePath.left(originalFilePath.lastIndexOf("/")));
				if (!originalDir.rename(hiddenFilePath, originalFilePath)) {
					qWarning() << "Failed to unhide " << hiddenFilePath;
				}
			}

			// Clean up empty directories. These happen when every file in a folder was blacklisted
			DeleteEmptyDirs(hiddenDir);
			});

		return true;
	}

	// Recursive to be depth first. rmdir only removes empty directories
	void HideLooseAssetService::DeleteEmptyDirs(const QDir& dir) const
	{
		for (const auto& dirName : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
		{
			DeleteEmptyDirs(dir.absoluteFilePath(dirName));
		}
		dir.rmdir(dir.absolutePath());
	}
}
