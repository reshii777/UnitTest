#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Интерфейс подключения к базе данных
class DBConnection {
public:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void execQuery(const std::string& query) = 0;
};

// Класс, использующий подключение к базе данных
class ClassThatUsesDB {
private:
    DBConnection* connection;

public:
    ClassThatUsesDB(DBConnection* conn) : connection(conn) {}

    void openConnection() {
        connection->open();
    }

    void useConnection() {
        connection->execQuery("SELECT * FROM table");
    }

    void closeConnection() {
        connection->close();
    }
};

// Мок-класс для тестирования
class MockDBConnection : public DBConnection {
public:
    MOCK_METHOD0(open, void());
    MOCK_METHOD0(close, void());
    MOCK_METHOD1(execQuery, void(const std::string&));
};

// Тестирующий класс
class ClassThatUsesDBTest : public testing::Test {
protected:
    MockDBConnection mockConnection;
    ClassThatUsesDB classUsingMock{ &mockConnection };
};

// Тесты
TEST_F(ClassThatUsesDBTest, TestOpenConnection) {
    EXPECT_CALL(mockConnection, open()).Times(1);

    classUsingMock.openConnection();
}

TEST_F(ClassThatUsesDBTest, TestUseConnection) {
    EXPECT_CALL(mockConnection, execQuery("SELECT * FROM table")).Times(1);

    classUsingMock.useConnection();
}

TEST_F(ClassThatUsesDBTest, TestCloseConnection) {
    EXPECT_CALL(mockConnection, close()).Times(1);

    classUsingMock.closeConnection();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
