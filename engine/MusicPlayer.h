#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMap>
#include <QString>
#include <QUrl>

// ============================================================
//  MusicPlayer — 背景音乐管理，支持淡入淡出
// ============================================================
class MusicPlayer : public QObject {
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer();

    // 注册音乐键和文件路径
    void registerTrack(const QString &key, const QString &filePath);

    // 播放（自动淡入）
    void play(const QString &key);

    // 停止（自动淡出）
    void stop();

    // 音量控制 [0,100]
    void  setVolume(int vol);
    int   volume() const { return m_volume; }

    // 当前播放
    QString currentKey() const { return m_currentKey; }
    bool    isPlaying()  const;

    // 循环播放
    void setLoop(bool loop);

    // 静音
    void setMuted(bool muted);
    bool isMuted() const { return m_muted; }

private:
    QMediaPlayer  *m_player      = nullptr;
    QAudioOutput  *m_audioOutput = nullptr;
    QMap<QString, QString> m_tracks;  // key → file path
    QString m_currentKey;
    int     m_volume = 60;
    bool    m_muted  = false;
    bool    m_loop   = true;
};
