#include <gtest/gtest.h>

#include "LoggingManager.h"
using namespace Log;

class ManagerTest:
    public Manager,
    public ::testing::Test
{
protected:
    void SetUp()
    {
        DeleteAll();
    }
    void TearDown()
    {
    }

protected:
    ManagerTest() {}
    virtual ~ManagerTest() {}
};



TEST_F(ManagerTest, GetRemove)
{
    Logger a("A");
    EXPECT_FALSE(remove(&a));

    Logger* aa = get("A");
    EXPECT_EQ(aa, get("A"));

    //同じ名前を持つ外loggerを渡しても失敗する
    //取得したポインタを確認しているので
    EXPECT_FALSE(remove(&a));

    //2回めの削除は失敗する、もうないので
    EXPECT_TRUE( remove(aa));
    EXPECT_FALSE(remove(aa));

    //delete後なので死ぬ
    //aa->critical("death!"); //Segmantation fault
    //ASSERT_DEATHはassertしないと引っかからない
    ASSERT_EXIT(
            (aa->critical("death!"), exit(0)),
            ::testing::ExitedWithCode(0),
            ".*");
}



TEST_F(ManagerTest, setFormatAll)
{
    Logger* a = get("A");
    Logger* b = get("B");

    setFormatAll("{message}");
    EXPECT_STREQ("{message}", a->getFormat());
    EXPECT_STREQ("{message}", b->getFormat());

    setFormatAll("{level}");
    EXPECT_STREQ("{level}", a->getFormat());
    EXPECT_STREQ("{level}", b->getFormat());
}

TEST_F(ManagerTest, setOutputAll)
{
    Logger* a = get("A");
    Logger* b = get("B");

    setOutputAll(std::cout);
    EXPECT_EQ(&std::cout, a->getOutput());
    EXPECT_EQ(&std::cout, b->getOutput());

    setOutputAll(std::cerr);
    EXPECT_EQ(&std::cerr, a->getOutput());
    EXPECT_EQ(&std::cerr, b->getOutput());
}


TEST_F(ManagerTest, DeleteAll)
{
    Logger* a = get("A");
    Logger* b = get("B");
    EXPECT_STREQ("A", a->getName());
    EXPECT_STREQ("B", b->getName());
    EXPECT_EQ(2, (int)loggers.size());

    ViewAll();

    DeleteAll();
    EXPECT_EQ(0, (int)loggers.size());
   
    /* 
    EXPECT_EXIT(
            (a->error("death!"), exit(0)),
            ::testing::ExitedWithCode(0),
            ".*");

    EXPECT_EXIT(
            (b->error("death!"), exit(0)),
            ::testing::ExitedWithCode(0),
            ".*");
    */
}


#include <thread>

void thread1(ManagerTest* p)
{
    p->get("thread1_a")->error("test!");
    p->remove(p->get("thread1_b"));
    p->setFormatAll("thread1 {name} {message}");
    p->setOutputAll(std::cerr);
}

void thread2(ManagerTest* p)
{
    p->get("thread2_a")->error("test!");
    p->remove(p->get("thread2_b"));
    p->setFormatAll("thread2 {name} {message}");
    p->setOutputAll(std::cout);
}

TEST_F(ManagerTest, threads)
{
    std::thread th1(thread1, this);
    std::thread th2(thread2, this);

    get("main_a")->error("test!");
    remove(get("main_b"));
    setFormatAll("main {name} {message}");
    setOutputAll(std::cout);

    th1.join();
    th2.join();

    ViewAll();
}
