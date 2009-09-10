/////////////////////////////////////////////////////
// Generated By wxActiveRecordGenerator v 1.2.0-rc3
/////////////////////////////////////////////////////

#include "Books.h"

/** ACTIVE_RECORD **/
Books::Books():wxSqliteActiveRecord(){
}

Books::Books(const wxString& name,const wxString& server,const wxString& user,const wxString& password,const wxString& table):wxSqliteActiveRecord(name,server,user,password,table){
}

Books::Books(DatabaseLayer* database,const wxString& table):wxSqliteActiveRecord(database,table){
}

bool Books::Create(const wxString& name,const wxString& server,const wxString& user,const wxString& password,const wxString& table){
	return wxSqliteActiveRecord::Create(name,server,user,password,table);
}

BooksRow* Books::RowFromResult(DatabaseResultSet* result){
	BooksRow* row=new BooksRow(this);

	row->GetFromResult(result);

	return row;
}

BooksRow* Books::New(){
	BooksRow* newRow=new BooksRow(this);
	garbageRows.Add(newRow);
	return newRow;
}

bool Books::Delete(int key){
    PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("DELETE FROM %s WHERE id=?"),m_table.c_str()));
    if (!pStatement) return false;
    pStatement->SetParamInt(1,key);
    pStatement->ExecuteUpdate();
    return true;
}

BooksRow* Books::Id(int key){
    PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("SELECT * FROM %s WHERE id=?"),m_table.c_str()));
    pStatement->SetParamInt(1,key);
    if (!pStatement) return NULL;
    DatabaseResultSet* result= pStatement->ExecuteQuery();

    BooksRow* row = NULL;
    bool ok = result && result->Next();
    if (ok) {
        row = RowFromResult(result);
        garbageRows.Add(row);
    }
    m_database->CloseResultSet(result);
    m_database->CloseStatement(pStatement);
    return row;
}

BooksRow* Books::Where(const wxString& whereClause){
    wxString prepStatement = wxString::Format(wxT("SELECT * FROM %s WHERE %s"),m_table.c_str(),whereClause.c_str());
    PreparedStatement* pStatement=m_database->PrepareStatement(prepStatement);
    if (!pStatement) return NULL;
    DatabaseResultSet* result= pStatement->ExecuteQuery();

    BooksRow* row = NULL;
    bool ok = result && result->Next();
    if (ok) {
        row = RowFromResult(result);
        garbageRows.Add(row);
    }
    m_database->CloseResultSet(result);
    m_database->CloseStatement(pStatement);
    return row;
}

BooksRowSet* Books::WhereSet(const wxString& whereClause,const wxString& orderBy){
	BooksRowSet* rowSet = new BooksRowSet();

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

BooksRow::BooksRow():wxActiveRecordRow(){
	newRow=true;
}

BooksRow::BooksRow(Books* activeRecord):wxActiveRecordRow(activeRecord){
	newRow=true;
}

BooksRow::BooksRow(const BooksRow& src){
	if(&src==this)
		return;
	newRow=src.newRow;

	genres=src.genres;
	description=src.description;
	id=src.id;
	id_archive=src.id_archive;
	file_size=src.file_size;
	annotation=src.annotation;
	file_name=src.file_name;
	file_type=src.file_type;
	deleted=src.deleted;
	title=src.title;
	sha1sum=src.sha1sum;
	id_author=src.id_author;

}

BooksRow::BooksRow(DatabaseLayer* database,const wxString& table):wxActiveRecordRow(database,table){
	newRow=true;
}


BooksRow& BooksRow::operator=(const BooksRow& src){
	if(&src==this)
		return *this;
	newRow=src.newRow;

	genres=src.genres;
	description=src.description;
	id=src.id;
	id_archive=src.id_archive;
	file_size=src.file_size;
	annotation=src.annotation;
	file_name=src.file_name;
	file_type=src.file_type;
	deleted=src.deleted;
	title=src.title;
	sha1sum=src.sha1sum;
	id_author=src.id_author;


	return *this;
}

bool BooksRow::GetFromResult(DatabaseResultSet* result){

	newRow=false;
    genres=result->GetResultString(wxT("genres"));
	description=result->GetResultString(wxT("description"));
	id=result->GetResultInt(wxT("id"));
	id_archive=result->GetResultInt(wxT("id_archive"));
	file_size=result->GetResultInt(wxT("file_size"));
	annotation=result->GetResultString(wxT("annotation"));
	file_name=result->GetResultString(wxT("file_name"));
	file_type=result->GetResultString(wxT("file_type"));
	deleted=result->GetResultString(wxT("deleted"));
	title=result->GetResultString(wxT("title"));
	sha1sum=result->GetResultString(wxT("sha1sum"));
	id_author=result->GetResultInt(wxT("id_author"));


	return true;
}

bool BooksRow::Save(){
    if(newRow){
        PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("INSERT INTO %s (sha1sum,genres,description,id,id_archive,file_size,annotation,file_name,deleted,file_type,title,id_author) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)"),m_table.c_str()));
        if (!pStatement) return false;
        pStatement->SetParamString(1,sha1sum);
        pStatement->SetParamString(2,genres);
        pStatement->SetParamString(3,description);
        pStatement->SetParamInt(4,id);
        pStatement->SetParamInt(5,id_archive);
        pStatement->SetParamInt(6,file_size);
        pStatement->SetParamString(7,annotation);
        pStatement->SetParamString(8,file_name);
        pStatement->SetParamString(9,deleted);
        pStatement->SetParamString(10,file_type);
        pStatement->SetParamString(11,title);
        pStatement->SetParamInt(12,id_author);
        pStatement->RunQuery();
        m_database->CloseStatement(pStatement);
        newRow=false;
    }
    else{
        PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("UPDATE %s SET sha1sum=?,genres=?,description=?,id_archive=?,file_size=?,annotation=?,file_name=?,deleted=?,file_type=?,title=?,id_author=? WHERE id=?"),m_table.c_str()));
        if (!pStatement) return false;
        pStatement->SetParamString(1,sha1sum);
        pStatement->SetParamString(2,genres);
        pStatement->SetParamString(3,description);
        pStatement->SetParamInt(4,id_archive);
        pStatement->SetParamInt(5,file_size);
        pStatement->SetParamString(6,annotation);
        pStatement->SetParamString(7,file_name);
        pStatement->SetParamString(8,deleted);
        pStatement->SetParamString(9,file_type);
        pStatement->SetParamString(10,title);
        pStatement->SetParamInt(11,id_author);
        pStatement->SetParamInt(12,id);
        pStatement->RunQuery();
        m_database->CloseStatement(pStatement);
	}
	return true;
}

bool BooksRow::Delete(){
    PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT("DELETE FROM %s WHERE id=?"),m_table.c_str()));
    if (!pStatement) return false;
	pStatement->SetParamInt(1,id);
	pStatement->ExecuteUpdate();
	return true;
}

ArchivesRow* BooksRow::GetArchive(){
	ArchivesRow* owner= new ArchivesRow(m_database,wxT("archives"));
	PreparedStatement* pStatement=m_database->PrepareStatement(wxT("SELECT * FROM archives WHERE id=?"));
	pStatement->SetParamInt(1,id_archive);
	pStatement->SetParamInt(1,id_archive);
	DatabaseResultSet* result= pStatement->ExecuteQuery();

	if(!result->Next())
		return NULL;

	owner->GetFromResult(result);
	garbageRows.Add(owner);
	m_database->CloseResultSet(result);
	m_database->CloseStatement(pStatement);
	return owner;
}

AuthorsRowSet* BooksRow::GetAuthors(const wxString& orderBy){
	AuthorsRowSet* set= new AuthorsRowSet(m_database,wxT("authors"));
	wxString str_statement =wxT("SELECT * FROM authors WHERE id=?");
	if(!orderBy.IsEmpty()) str_statement+=wxT(" ORDER BY ")+orderBy;
	PreparedStatement* pStatement=m_database->PrepareStatement(str_statement);
    if (!pStatement) return set;
	pStatement->SetParamInt(1,id);
	DatabaseResultSet* result= pStatement->ExecuteQuery();

	while(result->Next()){
		AuthorsRow* toAdd=new AuthorsRow(m_database,wxT("authors"));
		toAdd->GetFromResult(result);
		set->Add(toAdd);
	}
	garbageRowSets.Add(set);
	m_database->CloseResultSet(result);
	m_database->CloseStatement(pStatement);
	return set;
}



/** END ACTIVE RECORD ROW **/

/** ACTIVE RECORD ROW SET **/

BooksRowSet::BooksRowSet():wxActiveRecordRowSet(){
}

BooksRowSet::BooksRowSet(wxActiveRecord* activeRecord):wxActiveRecordRowSet(activeRecord){
}

BooksRowSet::BooksRowSet(DatabaseLayer* database,const wxString& table):wxActiveRecordRowSet(database,table){
}

BooksRow* BooksRowSet::Item(unsigned long item){
	return (BooksRow*)wxActiveRecordRowSet::Item(item);
}

int BooksRowSet::CMPFUNC_genres(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->genres.Cmp((*m_item2)->genres);
}

int BooksRowSet::CMPFUNC_description(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->description.Cmp((*m_item2)->description);
}

int BooksRowSet::CMPFUNC_id(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	if((*m_item1)->id<(*m_item2)->id)
		return -1;
	else if((*m_item1)->id>(*m_item2)->id)
		return 1;
	else
		return 0;
}

int BooksRowSet::CMPFUNC_id_archive(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	if((*m_item1)->id_archive<(*m_item2)->id_archive)
		return -1;
	else if((*m_item1)->id_archive>(*m_item2)->id_archive)
		return 1;
	else
		return 0;
}

int BooksRowSet::CMPFUNC_file_size(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	if((*m_item1)->file_size<(*m_item2)->file_size)
		return -1;
	else if((*m_item1)->file_size>(*m_item2)->file_size)
		return 1;
	else
		return 0;
}

int BooksRowSet::CMPFUNC_annotation(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->annotation.Cmp((*m_item2)->annotation);
}

int BooksRowSet::CMPFUNC_file_name(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->file_name.Cmp((*m_item2)->file_name);
}

int BooksRowSet::CMPFUNC_file_type(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->file_type.Cmp((*m_item2)->file_type);
}

int BooksRowSet::CMPFUNC_deleted(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->deleted.Cmp((*m_item2)->deleted);
}

int BooksRowSet::CMPFUNC_title(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->title.Cmp((*m_item2)->title);
}

int BooksRowSet::CMPFUNC_sha1sum(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	return (*m_item1)->sha1sum.Cmp((*m_item2)->sha1sum);
}

int BooksRowSet::CMPFUNC_id_author(wxActiveRecordRow** item1,wxActiveRecordRow** item2){
	BooksRow** m_item1=(BooksRow**)item1;
	BooksRow** m_item2=(BooksRow**)item2;
	if((*m_item1)->id_author<(*m_item2)->id_author)
		return -1;
	else if((*m_item1)->id_author>(*m_item2)->id_author)
		return 1;
	else
		return 0;
}

CMPFUNC_proto BooksRowSet::GetCmpFunc(const wxString& var) const{
	if(var==wxT("genres"))
		return (CMPFUNC_proto)CMPFUNC_genres;
	else if(var==wxT("description"))
		return (CMPFUNC_proto)CMPFUNC_description;
	else if(var==wxT("id"))
		return (CMPFUNC_proto)CMPFUNC_id;
	else if(var==wxT("id_archive"))
		return (CMPFUNC_proto)CMPFUNC_id_archive;
	else if(var==wxT("file_size"))
		return (CMPFUNC_proto)CMPFUNC_file_size;
	else if(var==wxT("annotation"))
		return (CMPFUNC_proto)CMPFUNC_annotation;
	else if(var==wxT("file_name"))
		return (CMPFUNC_proto)CMPFUNC_file_name;
	else if(var==wxT("file_type"))
		return (CMPFUNC_proto)CMPFUNC_file_type;
	else if(var==wxT("deleted"))
		return (CMPFUNC_proto)CMPFUNC_deleted;
	else if(var==wxT("title"))
		return (CMPFUNC_proto)CMPFUNC_title;
	else if(var==wxT("sha1sum"))
		return (CMPFUNC_proto)CMPFUNC_sha1sum;
	else if(var==wxT("id_author"))
		return (CMPFUNC_proto)CMPFUNC_id_author;
	else
	return (CMPFUNC_proto)CMPFUNC_default;
}



/** END ACTIVE RECORD ROW SET **/

////@@begin custom implementations

////@@end custom implementations
