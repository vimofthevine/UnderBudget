package com.vimofthevine.underbudget.auth

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import io.ktor.application.*
import io.ktor.auth.*
import io.ktor.locations.*
import io.ktor.request.*
import io.ktor.response.*
import io.ktor.routing.*

import org.slf4j.LoggerFactory

@Location("/users")
class UsersEndpoint()

@Location("/users/{user}")
data class UserEndpoint(val user: UUID)

@Location("/tokens")
class TokensEndpoint()

data class UserInfoInput(val name: String, val email: String, val password: String)

data class RegistrationResponse(val error: String? = null, val userId: UUID? = null)
data class LoginResponse(val error: String? = null, val token: String? = null)

fun Routing.auth(db: DbService, passwords: Passwords, jwt: JwtService) {
	val logger = LoggerFactory.getLogger("underbudget.auth")

    post<UsersEndpoint> {
        val user = call.receive<UserInfoInput>()
        val response = db.transaction {
            if (user.name.length < 6) {
                RegistrationResponse(error = "Username must be at least 6 characters in length")
            } else if (user.name.length > 128) {
                RegistrationResponse(error = "Username must be less than 128 characters in length")
            } else if (!user.name.matches("[a-zA-Z0-9._]+".toRegex())) {
                RegistrationResponse(error = "Username must contain only letters, numbers, dots, or underscores")
            } else if (user.password.length < 12) {
                RegistrationResponse(error = "Password must be at least 12 characters in length")
            } else if (db.findUserByName(user.name) != null) {
                RegistrationResponse(error = "Username is already in use")
            } else if (db.findUserByEmail(user.email) != null) {
                RegistrationResponse(error = "User with given email address already exists")
            } else {
                val salt = passwords.generateSalt()
                val hash = passwords.hash(user.password, salt)
                try {
                    val userId = db.createUser(user.name, user.email, salt, hash)
                    RegistrationResponse(userId = userId)
                } catch (e: Throwable) {
                    logger.error("Failed to register user", e)
                    RegistrationResponse(error = "Unable to register user")
                }
            }
        }
        call.respond(response)
    }
    
    authenticate("jwt") {
        put<UserEndpoint> {
            // it.user
        }
    }
    
    post<TokensEndpoint> {
        val login = call.receive<UserPasswordCredential>()
        val response = db.transaction {
            val user = db.findUserByName(login.name)
            if (user == null) {
                logger.info("Attempt to login with invalid username, ${login.name}")
                LoginResponse(error = "Invalid login credentials")
            } else {
                val hash = passwords.hash(login.password, user.salt!!)
                if (hash != user.password) {
                    logger.info("Attempt to login as ${login.name} with invalid password")
                	LoginResponse(error = "Invalid login credentials")
                } else {
                    LoginResponse(token = jwt.createToken(user))
                }
            }
        }
        call.respond(response)
    }
}