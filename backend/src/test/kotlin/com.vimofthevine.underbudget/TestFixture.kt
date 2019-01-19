package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*
import com.vimofthevine.underbudget.ledger.setupLedgerTables

import io.ktor.application.*
import io.ktor.config.*
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
    val testUser = createUser("testUser", "user@test.com", "testPassword")
    val testUserId = testUser.id!!
    
    fun createUser(name: String, email: String, password: String) =
        transaction(dbSvc.db) {
            val id = dbSvc.createUser(User(
                id = null,
                name = name,
                email = email,
                salt = testSalt,
                hashedPassword = pwSvc.hash(password, testSalt)
            ))
            dbSvc.findUserById(id)!!
        }
    
    fun createToken(user: User = testUser): String {
        val token = jwtSvc.createToken(user)
        val decoded = jwtSvc.decode(token)
        transaction(dbSvc.db) {
            dbSvc.createToken(Token(
                id = null,
                jwtId = decoded.getId(),
                userId = user.id!!,
                issued = decoded.getIssuedAt(),
                subject = ""
            ))
        }
        return token
    }
    
    fun withServer(test: TestApplicationEngine.() -> Unit) =
        withTestApplication({
            (environment.config as MapApplicationConfig).apply {
                put("ktor.deployment.demo", "false")
                put("jwt.realm", "UnitTest")
            }
            main(dbService = dbSvc, passwdService = pwSvc, jwtService = jwtSvc)
        }, test)
    
    fun <T> withDatabase(setup: Boolean = true, body: DbService.() -> T): T =
    	transaction(dbSvc.db) {
            if (setup) {
            	setupAuthTables()
            	setupLedgerTables()
            }
            dbSvc.body()
        }
    
}

