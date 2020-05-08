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


            b2Body* ground = nullptr;
            {
                    b2BodyDef bd;
                    ground = m_world->CreateBody(&bd);

                    b2EdgeShape shape;

                    b2FixtureDef fd;
                    fd.shape = &shape;
                    fd.density = settings.ground_density;
                    fd.friction = settings.ground_friction;


                    const int size=20;  //settings.number_of_stages;
                    const float dx = settings.stage_width_x;


                    //start is flat, at small plate, size 2*dx
                    shape.Set(b2Vec2(-2*dx, 0.0f), b2Vec2(2*dx, 0.0f));
                    ground->CreateFixture(&fd);

                    float x = 2*dx;
                    float y1 = 0.0f;


                    //można potem zamienić na wektor, jak zdecydujemy
                    //gdzie to przechowujemy
                    float hs[size] = {1.0f, 3.0f, 4.0f, -1.0f, -2.0f, 1.0f,
                                      2.0f, 5.0f, -1.25f, 0.0f,1.0f, 1.0f,
                                      4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -5.0f,
                                      -5.25f, 0.0f};


                     // float hs[size];



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

                    x += 2*dx;
                    shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 2*dx));
                    ground->CreateFixture(&fd);

            }
}



std::vector<cer::physics::Car> cer::physics::World::generateCars(const cer::CarsPopulationData &population,
                                       cer::SimulationData *simulation_data){


    //number of cars in simulation
    static const size_t car_No=population.cars().carsNum();
    static const int BODY_POINTS_NUM = population.cars().parametersNum()-2;

    //vector of cars
    std::vector<Car> cars;

    std::vector<double>::iterator it;


    //for each car
    for(size_t i=0;i<car_No;i++){


        Car car_t; //temporary object

        //radiuses of wheels are saved
        it=population.cars().begin(i);
        car_t.circle_front.m_radius = *it;
        it++;
        car_t.circle_rear.m_radius = *it;
        it++;


        //coordinates of wheels are set later
        double wheel1_x=0;
        double wheel1_y=0;

        double wheel2_x=0;
        double wheel2_y=0;



        //shape of a car

        int j=0;
        for( it,j;it<population.cars().end(i) && j<BODY_POINTS_NUM;it+=2,j++){
            car_t.vertices[j].Set(*it,*(it+1));

            /*there is no method for getting coordinates from vertices
            so it is better to save them*/
            if(j==settings.wheel1_point_no){
                wheel1_x=*it;
                wheel1_y=*(it+1);
            }

            if(j==settings.wheel2_point_no){
                wheel2_x=*it;
                wheel2_y=*(it+1);
            }


        }

        car_t.chassis.Set(car_t.vertices, BODY_POINTS_NUM);




        //jezeli koło nie bedzie sie krecic to ten blok skopiowac
        //do wheel 2

        car_t.bd.type = b2_dynamicBody;
        car_t.bd.position.Set(0.0f, 1.0f);
        m_car = m_world->CreateBody(&car_t.bd);

        b2Vec2 axis(0.0f, 1.0f);



        //wheel 1
        m_car->CreateFixture(&car_t.chassis, 1.0f);
        car_t.fd.shape = &car_t.circle_front;
        car_t.fd.density = settings.wheel1_density;
        car_t.fd.friction = settings.wheel1_friction;

        car_t.bd.position.Set(wheel1_x, wheel1_y);  //wheel position
        m_wheel1 = m_world->CreateBody(&car_t.bd);
        m_wheel1->CreateFixture(&car_t.fd);

        car_t.jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
        car_t.jd.motorSpeed = settings.motor1_speed;
        car_t.jd.maxMotorTorque = settings.motor1_maxTorque;
        car_t.jd.enableMotor = settings.motor1_enable;
        car_t.jd.frequencyHz = settings.motor1_frequencyHz;
        car_t.jd.dampingRatio = settings.wheel1_dampingRatio;
        m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&car_t.jd);



        //wheel 2
        m_car->CreateFixture(&car_t.chassis, 1.0f);
        car_t.fd.shape = &car_t.circle_rear;
        car_t.fd.density = settings.wheel2_density;
        car_t.fd.friction = settings.wheel2_friction;

        car_t.bd.position.Set(wheel2_x, wheel2_y);    //wheel position
        m_wheel2 = m_world->CreateBody(&car_t.bd);
        m_wheel2->CreateFixture(&car_t.fd);

        car_t.jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
        car_t.jd.motorSpeed = settings.motor2_speed;
        car_t.jd.maxMotorTorque = settings.motor2_maxTorque;
        car_t.jd.enableMotor = settings.motor2_enable;
        car_t.jd.frequencyHz = settings.motor2_frequencyHz;
        car_t.jd.dampingRatio = settings.wheel2_dampingRatio;
        m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&car_t.jd);


        cars.push_back(car_t);
    }

    return cars;

}




bool cer::physics::World::runSimulation(std::vector<cer::physics::Car> cars) {

    /* for dummy simulation
    auto cars_num = population_.cars().carsNum();
    simulation_data_->reset(cars_num);
    */

    /*
    // dummy simulation
    std::vector<float> speeds(cars_num);
    for (auto &s : speeds) {
      s = float(rand() % 10);
    }
    */


#if defined(_WIN32)
// Enable memory-leak reports
_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif


  float timeStep = settings.m_hertz > 0.0f ? 1.0f / settings.m_hertz : float(0.0f);

  //m_world->SetAllowSleeping(settings.m_enableSleep);
  m_world->SetWarmStarting(settings.m_enableWarmStarting);
  m_world->SetContinuousPhysics(settings.m_enableContinuous);
  m_world->SetSubStepping(settings.m_enableSubStepping);



  bool stop=0; //flag for simulation control

  while(!stop) {
    // simulate computations
    //    logger::info() << "runSimulation" << j;



      for (size_t i = 0; i < cars_num; ++i) {

          b2Vec2 position = &cars[i].bd->GetPosition();
          float angle = &cars[i].bd->GetAngle();

          m_world->Step(timeStep, settings.m_velocityIterations, settings.m_positionIterations);


          // Now print the position and angle of the body.
          position = body->GetPosition();
          angle = body->GetAngle();



        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        simulation_data_->pushPosition(
            i, Position{(1.5f * speeds[i] + 1.0f) * j,
                        (1.0f + 0.5f * speeds[i]) * j, speeds[i] * 0.1f * j});


        int k=0;
        if(true /*pozycja sie nie zmienia*/)
            k++;
        else
            k=0;

        int max_iter=10000;


        bool flag=0;
        if(k>max_iter)  //flaga up
            flag=1;





    }
  }


  return true;
}




float cer::physics::World::vecNorm() const { return vec_.Length(); }
