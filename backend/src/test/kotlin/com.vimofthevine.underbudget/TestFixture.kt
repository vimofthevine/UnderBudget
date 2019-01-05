package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*

import io.ktor.application.*
import io.ktor.server.testing.*

import org.jetbrains.exposed.sql.Database
import org.jetbrains.exposed.sql.transactions.transaction

open class TestFixture {
    val dbSvc =  DbService(Database.connect(url = "jdbc:h2:mem:dev;DB_CLOSE_DELAY=-1",
                                            driver = "org.h2.Driver"))
    init {
        transaction(dbSvc.db) {
            exec("DROP ALL OBJECTS")
            setupAuthTables()
        }
    }
    
    val pwSvc = Passwords(10)
    val jwtSvc = JwtService("testIssuer", "testSecret")
    
    val testSalt = pwSvc.generateSalt()
    val testUserId = transaction(dbSvc.db) {
        dbSvc.createUser("Test User", "user@test.com", testSalt,
                         pwSvc.hash("testPassword", testSalt))
    }
    val testUser = transaction(dbSvc.db) {
        dbSvc.getUser(testUserId)
    }
    
    fun withServer(test: TestApplicationEngine.() -> Unit) =
        withTestApplication({ main(dbService = dbSvc, passwdService = pwSvc, jwtService = jwtSvc) },
                            test)
}

