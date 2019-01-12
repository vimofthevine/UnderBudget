package com.vimofthevine.underbudget.auth

import com.auth0.jwt.*
import com.auth0.jwt.algorithms.*
import com.auth0.jwt.interfaces.*

import io.ktor.application.*
import io.ktor.util.*

import java.util.Date

class JwtService(private val issuer: String, private val secret: String) {
    private val algorithm = Algorithm.HMAC512(secret)
    
    val verifier: JWTVerifier = JWT
    	.require(algorithm)
    	.withIssuer(issuer)
    	.build()
    
    fun createToken(user: User): String = JWT.create()
    	.withSubject("Authentication")
    	.withIssuer(issuer)
    	.withIssuedAt(Date())
    	.withJWTId(generateNonce())
    	.withClaim("userId", user.id.toString())
    	.sign(algorithm)
    
    fun decode(token: String): DecodedJWT = JWT.decode(token)
}

val Application.jwtRealm get() = environment.config.property("jwt.realm").getString()

fun Application.createJwtService(): JwtService =
	JwtService(
        environment.config.property("jwt.issuer").getString(),
        environment.config.property("jwt.secret").getString()
    )