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

package com.vimofthevine.underbudget.file.transaction;

import java.io.InputStream;
import java.util.List;

import org.simpleframework.xml.Serializer;

import com.vimofthevine.underbudget.core.budget.period.BudgetingPeriod;
import com.vimofthevine.underbudget.core.ledger.Ledger;
import com.vimofthevine.underbudget.core.ledger.transaction.Transaction;
import com.vimofthevine.underbudget.core.progress.ProgressMonitorInputStream;
import com.vimofthevine.underbudget.core.progress.TaskProgress;
import com.vimofthevine.underbudget.file.xml.TransactionSerializerFactory;

/**
 * The <code>TransactionFileImporter</code> imports transactions
 * from an UnderBudget transaction export file.
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public class TransactionFileImporter implements TransactionImporter {

    @Override
    public List<Transaction> read(InputStream stream, BudgetingPeriod period,
            TaskProgress progress) throws TransactionImportException
    {
		try
		{
			progress.reset();
			
			Serializer serializer = TransactionSerializerFactory.createImportSerializer(period);
			
			Ledger ledger = serializer.read(Ledger.class, new ProgressMonitorInputStream(
				progress, stream));
			
			progress.complete();
			
			return ledger.getTransactionList();
		}
		catch (Exception e)
		{
			throw new TransactionImportException("Unable to parse transactions", e);
		}
    }

}
