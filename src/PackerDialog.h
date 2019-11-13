#ifndef PACKERDIALOG_H
#define PACKERDIALOG_H

#include <bsapacker/IPackerDialog.h>
#include <bsapacker/IModContext.h>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>


namespace BsaPacker
{
	class PackerDialog : public IPackerDialog
	{
		Q_OBJECT
			Q_INTERFACES(BsaPacker::IPackerDialog)
	public:
		explicit PackerDialog(const IModContext* modContext);
		~PackerDialog() override = default;

		// IPackerDialog interface
		void RefreshModList() override;
		[[nodiscard]] QString SelectedMod() const override;
		[[nodiscard]] QString SelectedPluginItem() const override;
		void UpdateNameList(const QString&) override;
		void RefreshSelectedName() override;

	public Q_SLOTS:
		void RefreshOkButton() override;

	private:
		const IModContext* m_ModContext = nullptr;

		QComboBox comboModList;
		QLabel labelChooseMod;
		QLabel labelChooseName;
		QListWidget listArchiveNames;
		QVBoxLayout layoutVertical;
		QHBoxLayout layoutHorizontal;
		QDialogButtonBox buttonsOkCancelMod;
	};
} // namespace BsaPacker

#endif // PACKERDIALOG_H
