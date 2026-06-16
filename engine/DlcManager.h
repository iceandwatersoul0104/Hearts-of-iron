#pragma once
#include <QObject>
#include <QDir>
#include <QList>
#include "DlcTypes.h"

class DlcManager : public QObject {
    Q_OBJECT
public:
    explicit DlcManager(QObject *parent = nullptr);

    // 扫描 dlc/ 目录，返回所有发现的 DLC 清单
    void scanDirectory(const QString &dlcRootDir);

    // 获取已扫描的 DLC
    const QList<DlcManifest> &manifests() const { return m_manifests; }

    // 根据 dlcId 获取清单
    const DlcManifest *getManifest(const QString &dlcId) const;

    // 加载指定 DLC 的某一章节
    bool loadChapter(const QString &dlcBasePath,
                     const QString &chapterFile,
                     DlcChapter &outChapter);

    // 获取 DLC 文件夹的绝对路径
    QString dlcBasePath(const QString &dlcId) const { return m_dlcRootDir + "/" + dlcId; }

signals:
    void dlcScanComplete(int count);

private:
    DlcManifest parseManifest(const QString &path);
    QStringList validateManifest(const DlcManifest &m, const QString &basePath);
    StoryNode  parseNodeFromJson(const QJsonObject &obj);

    QString m_dlcRootDir;
    QList<DlcManifest> m_manifests;
};
