#include "DlcManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

DlcManager::DlcManager(QObject *parent) : QObject(parent) {}

void DlcManager::scanDirectory(const QString &dlcRootDir) {
    m_dlcRootDir = dlcRootDir;
    m_manifests.clear();

    QDir root(dlcRootDir);
    if (!root.exists()) {
        qWarning() << "DLC directory not found:" << dlcRootDir;
        emit dlcScanComplete(0);
        return;
    }

    const auto entries = root.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        QString manifestPath = entry.absoluteFilePath() + "/manifest.json";
        if (!QFile::exists(manifestPath)) {
            qWarning() << "No manifest.json in" << entry.absoluteFilePath() << ", skipping";
            continue;
        }

        DlcManifest m = parseManifest(manifestPath);
        m.valid = true;

        QString basePath = entry.absoluteFilePath();
        QStringList errors = validateManifest(m, basePath);
        if (!errors.isEmpty()) {
            m.valid = false;
            m.errors = errors;
        }

        m_manifests.append(m);
    }

    emit dlcScanComplete(m_manifests.size());
}

DlcManifest DlcManager::parseManifest(const QString &path) {
    DlcManifest m;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        m.valid = false;
        m.errors.append(QStringLiteral("Cannot open manifest.json"));
        return m;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        m.valid = false;
        m.errors.append(QStringLiteral("Invalid JSON in manifest.json"));
        return m;
    }

    QJsonObject root = doc.object();

    m.dlcId       = root["dlcId"].toString();
    m.title       = root["title"].toString();
    m.subtitle    = root["subtitle"].toString();
    m.author      = root["author"].toString();
    m.version     = root["version"].toString();
    m.startChapter = root["startChapter"].toString();

    // Parse classes
    for (const auto &v : root["classes"].toArray()) {
        QJsonObject co = v.toObject();
        DlcClass dc;
        dc.id   = co["id"].toString();
        dc.name = co["name"].toString();
        dc.desc = co["desc"].toString();
        m.classes.append(dc);
    }

    // Parse chapters
    for (const auto &v : root["chapters"].toArray()) {
        QJsonObject cho = v.toObject();
        DlcChapterMeta cm;
        cm.id       = cho["id"].toString();
        cm.file     = cho["file"].toString();
        cm.name     = cho["name"].toString();
        cm.subtitle = cho["subtitle"].toString();
        cm.unlock   = cho["unlock"].toString();
        m.chapters.append(cm);
    }

    // Parse music map
    QJsonObject musicObj = root["music"].toObject();
    for (auto it = musicObj.begin(); it != musicObj.end(); ++it) {
        m.music[it.key()] = it.value().toString();
    }

    return m;
}

QStringList DlcManager::validateManifest(const DlcManifest &m, const QString &basePath) {
    QStringList errors;

    if (m.dlcId.isEmpty())
        errors.append(QStringLiteral("dlcId is empty"));
    if (m.title.isEmpty())
        errors.append(QStringLiteral("title is empty"));
    if (m.classes.isEmpty())
        errors.append(QStringLiteral("classes[] is empty — at least one class required"));
    if (m.chapters.isEmpty())
        errors.append(QStringLiteral("chapters[] is empty — at least one chapter required"));

    // Check class ID uniqueness
    QSet<QString> classIds;
    for (const auto &c : m.classes) {
        if (classIds.contains(c.id))
            errors.append(QStringLiteral("Duplicate class id: ") + c.id);
        classIds.insert(c.id);
    }

    // Check chapter ID uniqueness + file existence
    QSet<QString> chapterIds;
    for (const auto &ch : m.chapters) {
        if (chapterIds.contains(ch.id))
            errors.append(QStringLiteral("Duplicate chapter id: ") + ch.id);
        chapterIds.insert(ch.id);

        QString fullPath = basePath + "/" + ch.file;
        if (!QFile::exists(fullPath))
            errors.append(QStringLiteral("Chapter file not found: ") + ch.file);
    }

    // startChapter must exist
    if (!m.startChapter.isEmpty() && !chapterIds.contains(m.startChapter))
        errors.append(QStringLiteral("startChapter '") + m.startChapter +
                      QStringLiteral("' not found in chapters[]"));

    // Non-start chapters' unlock must reference existing chapter
    for (const auto &ch : m.chapters) {
        if (ch.unlock != "start" && !chapterIds.contains(ch.unlock))
            errors.append(QStringLiteral("Chapter '") + ch.id +
                          QStringLiteral("' unlock target '") + ch.unlock +
                          QStringLiteral("' not found"));
    }

    return errors;
}

const DlcManifest *DlcManager::getManifest(const QString &dlcId) const {
    for (const auto &m : m_manifests) {
        if (m.dlcId == dlcId)
            return &m;
    }
    return nullptr;
}

bool DlcManager::loadChapter(const QString &dlcBasePath,
                              const QString &chapterFile,
                              DlcChapter &outChapter) {
    QString fullPath = dlcBasePath + "/" + chapterFile;
    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull())
        return false;

    QJsonObject root = doc.object();
    outChapter.chapterId    = root["chapterId"].toString();
    outChapter.startNodeId  = root["startNodeId"].toString();
    outChapter.defeatNodeId = root["defeatNodeId"].toString();

    // 解析职业专属起始节点 (可选字段，默认空映射)
    QJsonObject csnObj = root["classStartNodes"].toObject();
    for (auto it = csnObj.begin(); it != csnObj.end(); ++it) {
        outChapter.classStartNodes[it.key()] = it.value().toString();
    }

    QJsonArray nodesArr = root["nodes"].toArray();
    for (const auto &v : nodesArr) {
        StoryNode node = parseNodeFromJson(v.toObject());
        outChapter.nodes[node.id] = node;
    }

    return true;
}

StoryNode DlcManager::parseNodeFromJson(const QJsonObject &obj) {
    StoryNode n;
    n.id            = obj["id"].toString();
    n.locationTitle = obj["locationTitle"].toString();
    n.text          = obj["text"].toString();
    n.musicKey      = obj["musicKey"].toString();
    n.nextNodeId    = obj["nextNodeId"].toString();
    n.isVictory     = obj["isVictory"].toBool(false);
    n.isDefeat      = obj["isDefeat"].toBool(false);

    QString typeStr = obj["type"].toString();
    if (typeStr == "narrative")
        n.type = NodeType::Narrative;
    else if (typeStr == "ending")
        n.type = NodeType::Ending;
    else
        n.type = NodeType::Choice;

    // Parse choices
    for (const auto &cv : obj["choices"].toArray()) {
        QJsonObject co = cv.toObject();
        Choice c;
        c.text            = co["text"].toString();
        c.nextNodeId      = co["nextNodeId"].toString();
        c.hpDelta         = co["hpDelta"].toInt(0);
        c.moraleDelta     = co["moraleDelta"].toInt(0);
        c.isCombat        = co["isCombat"].toBool(false);
        c.combatThreshold = co["combatThreshold"].toInt(50);
        c.successNodeId   = co["successNodeId"].toString();
        c.failureNodeId   = co["failureNodeId"].toString();
        c.failHpDelta     = co["failHpDelta"].toInt(-20);
        c.failMoraleDelta = co["failMoraleDelta"].toInt(-15);
        c.classRestricted = co["classRestricted"].toBool(false);

        for (const auto &fv : co["requiredFlags"].toArray())
            c.requiredFlags.insert(fv.toString());
        for (const auto &fv : co["grantedFlags"].toArray())
            c.grantedFlags.insert(fv.toString());
        for (const auto &cv : co["allowedClasses"].toArray())
            c.allowedClasses.append(cv.toString());
        for (const auto &bv : co["bonusClasses"].toArray())
            c.bonusClasses.append(bv.toString());

        n.choices.append(c);
    }

    // Parse classText
    QJsonObject ctObj = obj["classText"].toObject();
    for (auto it = ctObj.begin(); it != ctObj.end(); ++it) {
        n.classText[it.key()] = it.value().toString();
    }

    return n;
}
