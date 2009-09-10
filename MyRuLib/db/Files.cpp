/////////////////////////////////////////////////////
// Generated By wxActiveRecordGenerator v 1.2.0-rc3
/////////////////////////////////////////////////////

#include "Files.h"

/** ACTIVE_RECORD **/
Files::Files():wxSqliteActiveRecord(){
}

Files::Files(const wxString& name,const wxString& server,const wxString& user,const wxString& password,const wxString& table):wxSqliteActiveRecord(name,server,user,password,table){
}

Files::Files(DatabaseLayer* database,const wxString& table):wxSqliteActiveRecord(database,table){
}

bool Files::Create(const wxString& name,const wxString& server,const wxString& user,const wxString& password,const wxString& table){
	return wxSqliteActiveRecord::Create(name,server,user,password,table);
}

FilesRow* Files::RowFromResult(DatabaseResultSet* result){
	FilesRow* row=new FilesRow(this);
	row->GetFromResult(result);
	return row;
}

FilesRow* Files::New(){
	FilesRow* newRow=new FilesRow(this);
	garbageRows.Add(newRow);
	return newRow;
}

FilesRowSet* Files::IdBook(int key){
	FilesRowSet* rowSet = new FilesRowSet();
	PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("SELECT * FROM %s WHERE id_book=?"),m_table.c_str()));
    if (!pStatement) return rowSet;
    DatabaseResultSet* result= pStatement->ExecuteQuery();

    if(result){
        while(result->Next()){
            rowSet->Add(RowFromResult(result));
        }
    }

	garbageRowSets.Add(rowSet);
	m_database->CloseResultSet(result);
	m_database->CloseStatement(pStatement);
	return rowSet;
}

FilesRow* Files::Where(const wxString& whereClause){
    wxString prepStatement = wxString::Format(wxT("SELECT * FROM %s WHERE %s"),m_table.c_str(),whereClause.c_str());
    PreparedStatement* pStatement=m_database->PrepareStatement(prepStatement);
    if (!pStatement) return NULL;
    DatabaseResultSet* result= pStatement->ExecuteQuery();

    FilesRow* row = NULL;
    bool ok = result && result->Next();
    if (ok) {
        row = RowFromResult(result);
        garbageRows.Add(row);
    }
    m_database->CloseResultSet(result);
    m_database->CloseStatement(pStatement);
    return row;
}

FilesRowSet* Files::WhereSet(const wxString& whereClause,const wxString& orderBy){
	FilesRowSet* rowSet = new FilesRowSet();

    wxString prepStatement=wxString::Format(wxT("SELECT * FROM %s WHERE %s"),m_table.c_str(),whereClause.c_str());
    if(!orderBy.IsEmpty()) prepStatement+=wxT(" ORDER BY ")+orderBy;
    PreparedStatement* pStatement=m_database->PrepareStatement(prepStatement);
    if (!pStatement) return rowSet;
    DatabaseResultSet* result= pStatement->ExecuteQuery();

    if(result){
        while(result->Next()){
            rowSet->Add(RowFromResult(result));
        }
    }

	garbageRowSets.Add(rowSet);
	m_database->CloseResultSet(result);
	m_database->CloseStatement(pStatement);
	return rowSet;
}

/** END ACTIVE RECORD **/

/** ACTIVE RECORD ROW **/

FilesRow::FilesRow():wxActiveRecordRow(){
	newRow=true;
}

FilesRow::FilesRow(Files* activeRecord):wxActiveRecordRow(activeRecord){
	newRow=true;
}

FilesRow::FilesRow(const FilesRow& src){
	if(&src==this)
		return;
	newRow=src.newRow;

	id_archive=src.id_archive;
	id_book=src.id_book;
	file_name=src.file_name;

}

FilesRow::FilesRow(DatabaseLayer* database,const wxString& table):wxActiveRecordRow(database,table){
	newRow=true;
}


FilesRow& FilesRow::operator=(const FilesRow& src){
	if(&src==this)
		return *this;
	newRow=src.newRow;

	id_archive=src.id_archive;
	id_book=src.id_book;
	file_name=src.file_name;


	return *this;
}

bool FilesRow::GetFromResult(DatabaseResultSet* result){

	newRow=false;
		id_archive=result->GetResultInt(wxT("id_archive"));
	id_book=result->GetResultInt(wxT("id_book"));
	file_name=result->GetResultString(wxT("file_name"));


	return true;
}


bool FilesRow::Save(){
    if(newRow){
        PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("INSERT INTO %s (id_archive,id_book,file_name) VALUES (?,?,?)"),m_table.c_str()));
        if (!pStatement) return false;
        pStatement->SetParamInt(1,id_archive);
        pStatement->SetParamInt(2,id_book);
        pStatement->SetParamString(3,file_name);
        pStatement->RunQuery();
        m_database->CloseStatement(pStatement);
        newRow=false;
    }
    else{
        PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("UPDATE %s SET file_name=? WHERE id_book=? AND id_archive=?"),m_table.c_str()));
        if (!pStatement) return false;
        pStatement->SetParamString(1,file_name);
        pStatement->SetParamInt(2,id_book);
        pStatement->SetParamInt(3,id_archive);
        pStatement->RunQuery();
        m_database->CloseStatement(pStatement);

    }
    return true;
}

bool FilesRow::Delete(){
    PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("DELETE FROM %s WHERE id_book=?"),m_table.c_str()));
    if (!pStatement) return false;
    pStatement->SetParamInt(1,id_book);
    pStatement->ExecuteUpdate();
    return true;
}

/** END ACTIVE RECORD ROW **/

/** ACTIVE RECORD ROW SET **/

FilesRowSet::FilesRowSet():wxActiveRecordRowSet(){
}

FilesRowSet::FilesRowSet(wxActiveRecord* activeRecord):wxActiveRecordRowSet(activeRecord){
}

FilesRowSet::FilesRowSet(DatabaseLayer* database,const wxString& table):wxActiveRecordRowSet(database,table){
}

FilesRow* FilesRowSet::Item(unsigned long item){
	return (FilesRow*)wxActiveRecordRowSet::Item(item);
}

int FilesRowSet::CMPFUNC_id_archive(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	FilesRow** m_item1=(FilesRow**)item1;
	FilesRow** m_item2=(FilesRow**)item2;
	if((*m_item1)->id_archive<(*m_item2)->id_archive)
		return -1;
	else if((*m_item1)->id_archive>(*m_item2)->id_archive)
		return 1;
	else
		return 0;
}

int FilesRowSet::CMPFUNC_id_book(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	FilesRow** m_item1=(FilesRow**)item1;
	FilesRow** m_item2=(FilesRow**)item2;
	if((*m_item1)->id_book<(*m_item2)->id_book)
		return -1;
	else if((*m_item1)->id_book>(*m_item2)->id_book)
		return 1;
	else
		return 0;
}

int FilesRowSet::CMPFUNC_file_name(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	FilesRow** m_item1=(FilesRow**)item1;
	FilesRow** m_item2=(FilesRow**)item2;
	return (*m_item1)->file_name.Cmp((*m_item2)->file_name);
}

CMPFUNC_proto FilesRowSet::GetCmpFunc(const wxString& var) const{
	if(var==wxT("id_archive"))
		return (CMPFUNC_proto)CMPFUNC_id_archive;
	else if(var==wxT("id_book"))
		return (CMPFUNC_proto)CMPFUNC_id_book;
	else if(var==wxT("file_name"))
		return (CMPFUNC_proto)CMPFUNC_file_name;
	else
	return (CMPFUNC_proto)CMPFUNC_default;
}



/** END ACTIVE RECORD ROW SET **/

////@@begin custom implementations

////@@end custom implementations
