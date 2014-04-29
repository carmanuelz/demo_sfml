#ifndef BLOODEMITTER_H
#define BLOODEMITTER_H

#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Vectors/PolarVector.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include <array>
#include <memory>

class BloodEmitter
{
    const sf::Int64	tailsPerExplosion	= 50;
	public:
		explicit BloodEmitter(sf::Vector2f position)
		: mAccumulatedTime(sf::Time::Zero)
		, mPosition(position)
		{
		}

		void operator() (thor::EmissionInterface& system, sf::Time dt)
		{
			const sf::Time tailInterval = sf::seconds(0.1f) / tailsPerExplosion;

			// Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
			mAccumulatedTime += dt;
			while (mAccumulatedTime - tailInterval > sf::Time::Zero)
			{
				emitBlood(system);
				mAccumulatedTime -= tailInterval;
			}
		}

	private:
		void emitBlood(thor::EmissionInterface& system)
		{
			// Create initial direction with random vector length and angle
			thor::PolarVector2f velocity(thor::random(70.f, 400.f), thor::random(200.f, 340.f));

			// Create particle at position of explosion, with emitter-specific color and at 80% initial scale
			thor::Particle particle(sf::seconds(0.5f));
			particle.position = mPosition;

            particle.velocity = velocity;
			system.emitParticle(particle);
		}

	private:
		sf::Time		mAccumulatedTime;
		sf::Vector2f	mPosition;
};

// Custom affector that fades particles out and accelerates them according to scale
class BloodkAffector
{
	public:
        const float	gravity = 80.f;
		void operator() (thor::Particle& particle, sf::Time dt)
		{
		    particle.velocity += dt.asSeconds() * sf::Vector2f(0.f, gravity) * particle.scale.x * particle.scale.y;
			particle.velocity *= 0.85f;
			if(thor::getRemainingRatio(particle)<0.4f)
                particle.color.a = static_cast<sf::Uint8>(256 * thor::getRemainingRatio(particle)/0.4f);
		}
};


#endif
