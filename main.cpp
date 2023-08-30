#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Класс имитирующий подключение к базе данных
class DVConnection {
public:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void execQuery(const std::string& query) = 0;
};

// Класс, использующий подключение к базе данных
class ClassThatUsesDB {
private:
    DVConnection* dbConnection;

public:
    ClassThatUsesDB(DVConnection* conn) : dbConnection(conn) {}

    void openConnection() {
        dbConnection->open();
    }

    void useConnection() {
        dbConnection->execQuery("SELECT * FROM some_table");
    }

    void closeConnection() {
        dbConnection->close();
    }
};

// Мок-класс для имитации подключения к базе данных
class MockDVConnection : public DVConnection {
public:
    MOCK_METHOD(void, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(void, execQuery, (const std::string& query), (override));
};

// Тесты
TEST(ClassThatUsesDBTest, TestDatabaseConnection) {
    MockDVConnection mockConnection;
    ClassThatUsesDB classUnderTest(&mockConnection);

    EXPECT_CALL(mockConnection, open())
        .Times(1);

    EXPECT_CALL(mockConnection, execQuery("SELECT * FROM some_table"))
        .Times(1);

    EXPECT_CALL(mockConnection, close())
        .Times(1);

    classUnderTest.openConnection();
    classUnderTest.useConnection();
    classUnderTest.closeConnection();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
