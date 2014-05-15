////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "AnimatedSprite.hpp"

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
    m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0), m_isPaused(paused), m_isLooped(looped), m_texture(NULL)
{

}

void AnimatedSprite::setAnimation(const Animation& animation)
{
    isTimeLine = false;
    m_animation = &animation;
    m_texture = m_animation->getSpriteSheet();
    if(!isReverse)
        m_currentFrame = 0;
    else
        m_currentFrame = m_animation->getSize()-1;
    setFrame(m_currentFrame);
}

void AnimatedSprite::setAnimationTransition(const Animation& animation)
{
    m_animation = &animation;
    m_texture = m_animation->getSpriteSheet();
    if(!isReverse)
        m_currentFrame = 0;
    else
        m_currentFrame = m_animation->getSize()-1;
    setFrame(m_currentFrame);
}

void AnimatedSprite::PrepareTimeLine()
{
    TimeLine.clear();
    indexRepetition = 0;
    indexTransition = 0;
    isStart = false;
    isEnd = false;
}

void AnimatedSprite::PushTransition(Transition* newTransition)
{
    TimeLine.push_back(newTransition);
}
void AnimatedSprite::StartTimeLine()
{
    setAnimationTransition(TimeLine[indexTransition]->animation);
    isTimeLine = true;
}

void AnimatedSprite::setFrameTime(sf::Time time)
{
    m_frameTime = time;
}

void AnimatedSprite::play()
{
    m_isPaused = false;
}

void AnimatedSprite::play(const Animation& animation)
{
    if (getAnimation() != &animation)
        setAnimation(animation);
    play();
}

void AnimatedSprite::pause()
{
    m_isPaused = true;
}

void AnimatedSprite::stop()
{
    m_isPaused = true;
    m_currentFrame = 0;
    setFrame(m_currentFrame);
}

void AnimatedSprite::setLooped(bool looped)
{
    m_isLooped = looped;
}

void AnimatedSprite::setColor(const sf::Color& color)
{
    // Update the vertices' color
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
}
sf::Color AnimatedSprite::getColor()
{
    return m_vertices[0].color;
}

const Animation* AnimatedSprite::getAnimation() const
{
    return m_animation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    sf::IntRect rect = m_animation->getFrame(m_currentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped() const
{
    return m_isLooped;
}

bool AnimatedSprite::isPlaying() const
{
    return !m_isPaused;
}

sf::Time AnimatedSprite::getFrameTime() const
{
    return m_frameTime;
}

void AnimatedSprite::FlipX()
{
    isFlipX = true;
}
void AnimatedSprite::unFlipX()
{
    isFlipX = false;
}

void AnimatedSprite::setReverse(bool band)
{
    isReverse = band;
}
void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime)
{
    if (m_animation)
    {
        //calculate new vertex positions and texture coordiantes
        sf::IntRect rect = m_animation->getFrame(newFrame);

        m_vertices[0].position = sf::Vector2f(0.f, 0.f);
        m_vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        m_vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        m_vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        if(!isFlipX)
        {
            m_vertices[0].texCoords = sf::Vector2f(left, top);
            m_vertices[1].texCoords = sf::Vector2f(left, bottom);
            m_vertices[2].texCoords = sf::Vector2f(right, bottom);
            m_vertices[3].texCoords = sf::Vector2f(right, top);
        }
        else
        {
            m_vertices[0].texCoords = sf::Vector2f(right, top);
            m_vertices[1].texCoords = sf::Vector2f(right, bottom);
            m_vertices[2].texCoords = sf::Vector2f(left, bottom);
            m_vertices[3].texCoords = sf::Vector2f(left, top);
        }

    }

    if (resetTime)
        m_currentTime = sf::Time::Zero;
}

void AnimatedSprite::update(sf::Time deltaTime)
{
    // if not paused and we have a valid animation
    if (!m_isPaused && m_animation)
    {
        // add delta time
        m_currentTime += deltaTime;

        // if current time is bigger then the frame time advance one frame
        if (m_currentTime >= m_frameTime)
        {
            // reset time, but keep the remainder
            m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

            if(!isReverse)
            {

                if(m_currentFrame == 0 && isTimeLine)
                {
                    updateTimeLineStart();
                }
                // get next Frame index
                if (m_currentFrame + 1 < m_animation->getSize())
                    m_currentFrame++;
                else
                {
                    if(isTimeLine)
                    {
                        updateTimeLineEnd();
                    }
                    // animation has ended
                    m_currentFrame = 0; // reset to start

                    if (!m_isLooped)
                    {
                        m_isPaused = true;
                    }
                }
            }
            else
            {
                if(m_animation->getSize()-1 == m_currentFrame && isTimeLine)
                {
                    updateTimeLineStart();
                }
                // get next Frame index
                if (m_currentFrame > 0)
                    m_currentFrame--;
                else
                {
                    // animation has ended
                    m_currentFrame = m_animation->getSize()-1; // reset to start
                    if(isTimeLine)
                    {
                        updateTimeLineEnd();
                    }
                    if (!m_isLooped)
                    {
                        m_isPaused = true;
                    }
                }

            }
            // set the current frame, not reseting the time
            setFrame(m_currentFrame, false);
        }
    }
}
void AnimatedSprite::updateTimeLineStart()
{
    if(indexTransition < TimeLine.size())
    {
        if(indexRepetition == 0 && TimeLine[indexTransition]->StartFunc != 0)
            TimeLine[indexTransition]->StartFunc();

        if(TimeLine[indexTransition]->beginRep != 0)
            TimeLine[indexTransition]->beginRep();
    }
}

void AnimatedSprite::updateTimeLineEnd()
{

    if(indexTransition < TimeLine.size())
    {
        if(TimeLine[indexTransition]->endRep != 0 )
            TimeLine[indexTransition]->endRep();
        if(indexRepetition+1 < TimeLine[indexTransition]->repetition)
        {
            indexRepetition+=1;
        }
        else
        {
            if(TimeLine[indexTransition]->EndFunc != 0 )
                TimeLine[indexTransition]->EndFunc();
            indexRepetition = 0;
            indexTransition +=1;
            if(indexTransition < TimeLine.size())
                setAnimationTransition(TimeLine[indexTransition]->animation);
        }
    }
    if(indexTransition == TimeLine.size())
    {
        isTimeLine = false;
    }
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_animation && m_texture)
    {
        states.transform *= getTransform();
        states.texture = m_texture;
        target.draw(m_vertices, 4, sf::Quads, states);
    }
}
