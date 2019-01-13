package com.vimofthevine.underbudget.auth

import com.auth0.jwt.interfaces.*
import com.vimofthevine.underbudget.DbService

import java.util.UUID

import io.ktor.application.*
import io.ktor.auth.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.request.*
import io.ktor.response.*
import io.ktor.routing.*

import org.slf4j.LoggerFactory

fun Routing.auth(db: DbService, passwords: Passwords, jwt: JwtService) {
	val logger = LoggerFactory.getLogger("underbudget.auth")

    post<UserResources> {
        val user = call.receive<UserData>()
        val response = db.transaction {
            if ((user.name == null) or (user.email == null) or (user.password == null)) {
                RegistrationResponse(error = "Missing required field(s)")
            } else if (user.name.length < 6) {
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
                    val userId = db.createUser(User(
                        id = null,
                        name = user.name,
                        email = user.email,
                        hashedPassword = hash,
                        salt = salt
                    ))
                    RegistrationResponse(userId = userId)
                } catch (e: Throwable) {
                    logger.error("Failed to register user", e)
                    RegistrationResponse(error = "Unable to register user")
                }
            }
        }
        call.respond(
            if (response.error == null) HttpStatusCode.Created else HttpStatusCode.BadRequest,
            response)
    }
    
    post<TokenResources> {
        val login = call.receive<UserPasswordCredential>()
        val response = db.transaction {
            if ((login.name == null) or (login.password == null)) {
                LoginResponse(error = "Missing required field(s)")
            } else {
                val user = db.findUserByName(login.name)
                if (user == null) {
                    logger.info("Attempt to login with invalid username, ${login.name}")
                    LoginResponse(error = "Invalid login credentials")
                } else {
                    val hash = passwords.hash(login.password, user.salt)
                    if (hash != user.hashedPassword) {
                        logger.info("Attempt to login as ${login.name} with invalid password")
                        LoginResponse(error = "Invalid login credentials")
                    } else {
                        val token = jwt.createToken(user)
                        val decoded = jwt.decode(token)
                        db.createToken(Token(
                            id = null,
                            jwtId = decoded.getId(),
                            userId = user.id!!,
                            issued = decoded.getIssuedAt(),
                            subject = ""
                        ))
                        LoginResponse(token = token)
                    }
                }
            }
        }
        call.respond(
            if (response.error == null) HttpStatusCode.Created else HttpStatusCode.BadRequest,
            response)
    }
    
    authenticate("jwt") {
        get<TokenResources> {
            call.userId?.let {
            	call.respond(Tokens(db.transaction { findTokensByUser(it) }))
            }
        }
        
        delete<TokenResource> {
            call.respond(db.transaction {
                val token = findTokenByJwtId(it.jwtId)
                if (token == null) {
                    HttpStatusCode.NotFound
                } else if (token.userId != call.userId) {
                    HttpStatusCode.Forbidden
                } else {
                	deleteTokenByJwtId(token.jwtId)
                	HttpStatusCode.OK
                }
            })
        }
        
        put<UserResource> {
            // it.user
        }
    }
    
}