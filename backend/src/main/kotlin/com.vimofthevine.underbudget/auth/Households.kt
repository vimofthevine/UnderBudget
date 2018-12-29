package com.vimofthevine.underbudget.auth

import java.util.UUID

import org.jetbrains.exposed.dao.EntityID
import org.jetbrains.exposed.dao.UUIDEntity
import org.jetbrains.exposed.dao.UUIDEntityClass
import org.jetbrains.exposed.dao.UUIDTable
import org.jetbrains.exposed.sql.SchemaUtils

object Households : UUIDTable("household") {
    val name = varchar("name", 128)
    val time = datetime("time")
}

class Household(id: EntityID<UUID>) : UUIDEntity(id) {
    companion object : UUIDEntityClass<Household>(Households)
    
    var name by Households.name
    var time by Households.time
}

fun setupAuthTables() : Unit {
    SchemaUtils.create(Households)
}