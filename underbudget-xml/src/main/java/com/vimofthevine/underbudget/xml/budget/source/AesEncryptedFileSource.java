/*
 * Copyright 2012 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.vimofthevine.underbudget.xml.budget.source;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.security.MessageDigest;
import java.security.SecureRandom;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.Budget;
import com.vimofthevine.underbudget.core.budget.source.BudgetSource;
import com.vimofthevine.underbudget.core.budget.source.BudgetSourceException;
import com.vimofthevine.underbudget.xml.budget.XmlBudget;

/**
 * Source for a budget stored in an encrypted file.
 * The unencrypted budget is stored as an XML budget.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class AesEncryptedFileSource implements BudgetSource {
	
	/**
	 * Magic number for identifying encrypted files
	 */
	public static final int MAGIC_NUMBER = 0x3b7d68a6;
	
	/**
	 * Number of bytes used for the magic number
	 */
	public static final int MAGIC_NUMBER_SIZE = 4;
	
	/**
	 * Log handle
	 */
	private static final Logger logger = Logger.getLogger(AesEncryptedFileSource.class.getName());
	
	/**
	 * Encrypted file
	 */
	private final File encFile;
	
	/**
	 * Encryption passkey
	 */
	private final String passkey;
	
	/**
	 * Simple XML serializer
	 */
	private final Serializer serializer;
	
	/**
	 * Budget from the source
	 */
	private XmlBudget budget;
	
	/**
	 * Constructs a new encrypted file budget source.
	 * 
	 * @param file     encrypted budget file
	 * @param password encryption passkey
	 */
	public AesEncryptedFileSource(File file, String password)
	{
		encFile = file;
		passkey = password;
		serializer = BudgetSerializerFactory.createSerializer();
	}
	
	/**
	 * Constructs a new encrypted file budget source,
	 * with a budget to be saved to the source.
	 * 
	 * @param file         encrypted budget file
	 * @param password     encryption passkey
	 * @param budgetToCopy budget to be saved
	 */
	public AesEncryptedFileSource(File file, String password, Budget budgetToCopy)
	{
		encFile = file;
		passkey = password;
		budget = (budgetToCopy instanceof XmlBudget)
			? (XmlBudget) budgetToCopy : new XmlBudget(budgetToCopy);
		serializer = BudgetSerializerFactory.createSerializer();
	}
	
	@Override
    public Budget retrieve() throws BudgetSourceException
    {
		logger.log(Level.FINE, "Reading " + encFile.getAbsolutePath());
		
		if ( ! encFile.exists())
		{
			throw new BudgetSourceException("File, " + encFile.getAbsolutePath()
				+ " does not exist");
    	}
		
		if ( ! encFile.canRead())
		{
			throw new BudgetSourceException("Cannot read file, "
				+ encFile.getAbsolutePath());
		}
		
		FileInputStream fileInStream = null;
		
		try
		{
			// Create contents of the file into a byte array
			fileInStream = new FileInputStream(encFile);
			byte[] encrypted = new byte[(int) encFile.length()];
			fileInStream.read(encrypted);
			
			// Use AES cipher
			Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
			
			// Get initialization vector from the file contents (skipping magic number)
			byte[] iv = new byte[cipher.getBlockSize()];
			System.arraycopy(encrypted, MAGIC_NUMBER_SIZE, iv, 0, iv.length);
			IvParameterSpec ivSpec = new IvParameterSpec(iv);
			
			// Get cipher text from the rest of the file contents (skipping magic number)
			byte[] ciphertext = new byte[encrypted.length - iv.length - MAGIC_NUMBER_SIZE];
			System.arraycopy(encrypted, MAGIC_NUMBER_SIZE + iv.length,
				ciphertext, 0, ciphertext.length);
			
			// Perform SHA-256 hash on passkey
			MessageDigest digest = MessageDigest.getInstance("SHA-256");
			digest.update(passkey.getBytes());
			// Grab first 16 bytes of the hash
			byte[] key = new byte[16];
			System.arraycopy(digest.digest(), 0, key, 0, key.length);
			SecretKeySpec keySpec = new SecretKeySpec(key, "AES");
			
			// Decrypt the cipher text
			cipher.init(Cipher.DECRYPT_MODE, keySpec, ivSpec);
			byte[] plaintext = cipher.doFinal(ciphertext);
			
			// Parse the plaintext budget XML
			ByteArrayInputStream byteStream = new ByteArrayInputStream(plaintext);
			budget = serializer.read(XmlBudget.class, byteStream);
			return budget;
		}
		catch (BadPaddingException bpe)
		{
			logger.log(Level.WARNING, "Unable to decrypt", bpe);
			throw new BudgetSourceException("Incorrect password, unable to decrypt "
				+ encFile.getAbsolutePath());
		}
		catch (Exception e)
		{
			throw new BudgetSourceException("Unable to open encrypted budget file", e);
		}
		finally
		{
			if (fileInStream != null)
			{
				try
				{
					fileInStream.close();
				}
				catch (IOException ioe)
				{
					logger.log(Level.WARNING, "Unable to close input stream", ioe);
				}
			}
		}
    }

	@Override
    public void persist() throws BudgetSourceException
    {
		logger.log(Level.FINE, "Saving to " + encFile.getAbsolutePath());
		
		if (budget == null)
			throw new BudgetSourceException("No budget to be saved");
		
		if (encFile.exists() && ! encFile.canWrite())
		{
			throw new BudgetSourceException("Cannot write to file, "
				+ encFile.getAbsolutePath());
		}
		
		FileOutputStream fileOutStream = null;
		
		try
		{
			// Write budget to XML
			ByteArrayOutputStream plaintext = new ByteArrayOutputStream();
			serializer.write(budget, plaintext);
			
			// Use AES cipher
			Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
			
			// Create random initialization vector
			byte[] iv = new byte[cipher.getBlockSize()];
			new SecureRandom().nextBytes(iv);
			IvParameterSpec ivSpec = new IvParameterSpec(iv);
			
			// Perform SHA-256 hash on passkey
			MessageDigest digest = MessageDigest.getInstance("SHA-256");
			digest.update(passkey.getBytes());
			// Grab first 16 bytes of the hash
			byte[] key = new byte[16];
			System.arraycopy(digest.digest(), 0, key, 0, key.length);
			SecretKeySpec keySpec = new SecretKeySpec(key, "AES");
			
			// Encrypt the budget XML
			cipher.init(Cipher.ENCRYPT_MODE, keySpec, ivSpec);
			byte[] ciphertext = cipher.doFinal(plaintext.toByteArray());
			
			// Combine magic number, initialization vector, and cipher text
			byte[] magicNumber = ByteBuffer.allocate(MAGIC_NUMBER_SIZE).putInt(MAGIC_NUMBER).array();
			byte[] encrypted = new byte[MAGIC_NUMBER_SIZE + iv.length + ciphertext.length];
			System.arraycopy(magicNumber, 0, encrypted, 0, MAGIC_NUMBER_SIZE);
			System.arraycopy(iv, 0, encrypted, MAGIC_NUMBER_SIZE, iv.length);
			System.arraycopy(ciphertext, 0, encrypted, MAGIC_NUMBER_SIZE + iv.length, ciphertext.length);
			
			// Write to file
			fileOutStream = new FileOutputStream(encFile);
			fileOutStream.write(encrypted);
		}
		catch (FileNotFoundException fnfe)
		{
			throw new BudgetSourceException("Could not create file, "
				+ encFile.getAbsolutePath());
		}
		catch (Exception e)
		{
			throw new BudgetSourceException("Unable to save encrypted budget file", e);
		}
		finally
		{
			if (fileOutStream != null)
			{
				try
				{
					fileOutStream.close();
				}
				catch (IOException ioe)
				{
					logger.log(Level.WARNING, "Unable to close output stream", ioe);
				}
			}
		}
    }

	@Override
    public String getDescription()
    {
		return (encFile == null) ? "New encrypted file" : encFile.getAbsolutePath();
    }

}
