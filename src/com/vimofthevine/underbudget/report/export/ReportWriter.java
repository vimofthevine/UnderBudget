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

package com.vimofthevine.underbudget.report.export;

import java.io.OutputStream;

import com.vimofthevine.underbudget.analysis.AnalysisResults;
import com.vimofthevine.underbudget.util.task.TaskProgress;

/**
 * Interface for report export writers
 * 
 * @author Kyle Treubig <kyle@vimofthevine.com>
 */
public interface ReportWriter {
	
	/**
	 * Returns a reference to the task progress updater
	 * for the parser. This allows for progress listeners
	 * to register with the parser for progress updates.
	 * 
	 * @return task progress updater for the parser
	 */
	public TaskProgress getProgress();
	
	/**
	 * Exports the analysis results to the given output stream
	 * 
	 * @param stream  output stream on which to write
	 * @param results analysis results to be written
	 * @throws ReportExportException if any error occurs
	 */
	public void write(OutputStream stream, AnalysisResults results)
	throws ReportExportException;

}
