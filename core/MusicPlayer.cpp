#include "MusicPlayer.h"
#include <QFileInfo>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_player      = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(m_volume / 100.0f);

    // 循环播放：结束后重播
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, [this](QMediaPlayer::MediaStatus status) {
        if (m_loop && status == QMediaPlayer::EndOfMedia) {
            m_player->setPosition(0);
            m_player->play();
        }
    });
}

MusicPlayer::~MusicPlayer() {}

void MusicPlayer::registerTrack(const QString &key, const QString &filePath) {
    m_tracks[key] = filePath;
}

void MusicPlayer::play(const QString &key) {
    if (key == m_currentKey && m_player->playbackState() == QMediaPlayer::PlayingState)
        return;

    if (!m_tracks.contains(key)) return;

    const QString &path = m_tracks[key];
    if (!QFileInfo::exists(path)) return;  // 文件不存在时静默跳过

    m_currentKey = key;
    m_player->setSource(QUrl::fromLocalFile(path));
    m_player->play();
}

void MusicPlayer::stop() {
    m_player->stop();
    m_currentKey.clear();
}

void MusicPlayer::setVolume(int vol) {
    m_volume = qBound(0, vol, 100);
    if (!m_muted)
        m_audioOutput->setVolume(m_volume / 100.0f);
}

bool MusicPlayer::isPlaying() const {
    return m_player->playbackState() == QMediaPlayer::PlayingState;
}

void MusicPlayer::setLoop(bool loop) {
    m_loop = loop;
}

void MusicPlayer::setMuted(bool muted) {
    m_muted = muted;
    m_audioOutput->setVolume(muted ? 0.0f : m_volume / 100.0f);
}
