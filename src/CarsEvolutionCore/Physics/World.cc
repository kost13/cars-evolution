#include "World.h"

#include <cpputils/logger.hpp>

#include "CarsPopulationData.h"
#include "SimulationData.h"

namespace logger = cpputils::log;


//static Settings s_settings;               //zm

cer::physics::World::World(const cer::CarsPopulationData &population,
                           cer::SimulationData *simulation_data)
    : population_(population), simulation_data_(simulation_data), vec_{3, 4} {


    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = std::make_unique<b2World>(gravity);

    m_pointCount = 0;
    m_stepCount = 0;

    b2BodyDef bodyDef;
    m_groundBody = m_world->CreateBody(&bodyDef);

    memset(&m_maxProfile, 0, sizeof(b2Profile));
    memset(&m_totalProfile, 0, sizeof(b2Profile));

    m_hz = 4.0f;
    m_zeta = 0.7f;
    m_speed = 50.0f;

            b2Body* ground = NULL;
            {
                    b2BodyDef bd;
                    ground = m_world->CreateBody(&bd);

                    b2EdgeShape shape;

                    b2FixtureDef fd;
                    fd.shape = &shape;
                    fd.density = 0.0f;
                    fd.friction = 0.6f;


                    const unsigned int size=20;

                    shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
                    ground->CreateFixture(&fd);

                    float hs[size] = {1.0f, 3.0f, 4.0f, -1.0f, -2.0f, 1.0f,
                                      2.0f, 5.0f, -1.25f, 0.0f,1.0f, 1.0f,
                                      4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -5.0f,
                                      -5.25f, 0.0f};


                     // float hs[size];

                    float x = 20.0f, y1 = 0.0f, dx = 10.0f;

                    for (unsigned int i = 0; i < size; ++i)
                    {
                            //h[i]=RandomFloat(-5.0f,5.0f)
                            float y2 = hs[i];
                            shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
                            ground->CreateFixture(&fd);
                            y1 = y2;
                            x += dx;
                    }

                    //wall at the end
                    //remove if track could be extended

                    x += 20.0f;
                    shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
                    ground->CreateFixture(&fd);

            }
}



void cer::physics::World::generateCars(const cer::CarsPopulationData &population,
                                       cer::SimulationData *simulation_data){


    //wpisać ilosc samochodów
    int N=20;

    //wpisać ilość punktów dla polygonu samochodu
    //jeżeli jest zmienna to to uwzględnić niżej!
    static const int BODY_POINTS_NUM = 8;

    //vector of cars
    std::vector<Car> cars(N);

    static const int container_size=2*BODY_POINTS_NUM+2;
    std::vector<double> carParameters(container_size);
    std::vector<double>::iterator it = population.carsVector().begin();


    //for each car
    for(int i=0;i<N;i++){


        //dobrze kopiuje?

        copy(it, it+container_size, carParameters.begin());
        it=it+container_size;

        //shape of a car
        for( int j=2;j<BODY_POINTS_NUM;j++)
            cars[i].vertices[j].Set(carParameters[j],carParameters[j+1] );

        cars[i].chassis.Set(cars[i].vertices, BODY_POINTS_NUM);



        //jezeli koło nie bedzie sie krecic to ten blok skopiowac
        //do wheel 2
        cars[i].circle.m_radius = carParameters[0];
        cars[i].bd.type = b2_dynamicBody;
        cars[i].bd.position.Set(0.0f, 1.0f);
        m_car = m_world->CreateBody(&cars[i].bd);

        m_car->CreateFixture(&cars[i].chassis, 1.0f);
        cars[i].fd.shape = &cars[i].circle;
        cars[i].fd.density = 1.0f;
        cars[i].fd.friction = 0.9f;


        cars[i].bd.position.Set(-1.0f, 0.35f);  //pozycje kół
        m_wheel1 = m_world->CreateBody(&cars[i].bd);
        m_wheel1->CreateFixture(&cars[i].fd);


        //jakby był problem z 2 kołem to sprawdzic ten blok
        cars[i].circle2.m_radius = carParameters[0];

        m_car->CreateFixture(&cars[i].chassis, 1.0f);
        cars[i].fd.shape = &cars[i].circle2;
        cars[i].fd.density = 1.0f;
        cars[i].fd.friction = 0.9f;

        cars[i].bd.position.Set(1.0f, 0.4f);    //pozycje kół
        m_wheel2 = m_world->CreateBody(&cars[i].bd);
        m_wheel2->CreateFixture(&cars[i].fd);
        //



        //wheel 1 parameters
        b2Vec2 axis(0.0f, 1.0f);
        cars[i].jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
        cars[i].jd.motorSpeed = 1.0f;
        cars[i].jd.maxMotorTorque = 20.0f;
        cars[i].jd.enableMotor = true;
        cars[i].jd.frequencyHz = m_hz;
        cars[i].jd.dampingRatio = m_zeta;

        //wheel 2 parameters
        m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&cars[i].jd);
        cars[i].jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
        cars[i].jd.motorSpeed = 1.0f;
        cars[i].jd.maxMotorTorque = 10.0f;
        cars[i].jd.enableMotor = false;
        cars[i].jd.frequencyHz = m_hz;
        cars[i].jd.dampingRatio = m_zeta;
        m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&cars[i].jd);

    }

}




bool cer::physics::World::runSimulation() {
  auto cars_num = population_.cars().carsNum();
  simulation_data_->reset(cars_num);


// #########################################################

//chyba wyrzucić
#if defined(_WIN32)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif


//s_settings.Load();



  //dałbym funkcję inititate z kosntruktorem

  //w pętli funkcję step

  //ustawienia w settingsach

  // dummy simulation
  std::vector<float> speeds(cars_num);
  for (auto &s : speeds) {
    s = float(rand() % 10);
  }



  for (int j = 0; j < 100; ++j) {
    // simulate computations
    //    logger::info() << "runSimulation" << j;
    {
      for (size_t i = 0; i < cars_num; ++i) {


          //pętla
          //s_test->Step(s_settings);


        simulation_data_->pushPosition(
            i, Position{(1.5f * speeds[i] + 1.0f) * j,
                        (1.0f + 0.5f * speeds[i]) * j, speeds[i] * 0.1f * j});
      }
    }
  }
  return true;
}

float cer::physics::World::vecNorm() const { return vec_.Length(); }
