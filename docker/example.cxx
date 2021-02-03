#include <mysql.h>
#include <stdlib.h>
#include <cstdio>

static char *server_args[] = {
  "this_program",       /* this string is not used */
  "--datadir=./data/",
  "--key_buffer_size=32M"
};
static char *server_groups[] = {
  "embedded",
  "server",
  "this_program_SERVER",
  (char *)NULL
};

MYSQL *db_connect(const char *dbname);
void db_disconnect(MYSQL *db);
int db_do_query(MYSQL *db, const char *query);

int main(void) {
  if (mysql_library_init(sizeof(server_args) / sizeof(char *),
                        server_args, NULL)) {
    fprintf(stderr, "could not initialize MySQL client library\n");
    exit(1);
  }

  MYSQL *db = db_connect(NULL);
  char* query = "CREATE DATABASE test;";
  db_do_query(db, query);

  MYSQL *db2 = db_connect("test");
  query = "CREATE TABLE t0 (dim0 int dimension=1 tile_extent=\"10\", attr0 int NULL) ENGINE=mytile;";
  db_do_query(db2, query);

  query = "INSERT INTO t0 (dim0, attr0) VALUES (1, 1), (2, 2), (3, 3);";
  db_do_query(db2, query);

  query = "SELECT * FROM t0;";
  db_do_query(db2, query);

  mysql_close(db);
  mysql_close(db2);

  /* Use any MySQL API functions here */

  mysql_library_end();

  return EXIT_SUCCESS;
}

static void die(MYSQL *db, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  (void)putc('\n', stderr);
  if (db)
    db_disconnect(db);
  exit(EXIT_FAILURE);
}

MYSQL* db_connect(const char *dbname) {
  MYSQL *db = mysql_init(NULL);
  if (!db)
    die(db, "mysql_init failed: no memory");
  /*
   * Notice that the client and server use separate group names.
   * This is critical, because the server does not accept the
   * client's options, and vice versa.
   */
  mysql_options(db, MYSQL_READ_DEFAULT_GROUP, "test2_libmysqld_CLIENT");
  if (!mysql_real_connect(db, NULL, NULL, NULL, dbname, 0, NULL, 0))
    die(db, "mysql_real_connect failed: %s", mysql_error(db));

  return db;
}

void db_disconnect(MYSQL *db) {
  mysql_close(db);
}

int db_do_query(MYSQL *db, const char *query) {
  if (mysql_query(db, query) != 0)
    goto err;

  if (mysql_field_count(db) > 0)
  {
    MYSQL_RES   *res;
    MYSQL_ROW    row, end_row;
    int num_fields;

    if (!(res = mysql_store_result(db)))
      goto err;
    num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res)))
    {
      (void)fputs(">> ", stdout);
      for (end_row = row + num_fields; row < end_row; ++row)
        (void)printf("%s\t", row ? (char*)*row : "NULL");
      (void)fputc('\n', stdout);
    }
    (void)fputc('\n', stdout);
    mysql_free_result(res);
  }
  else
    (void)printf("Affected rows: %lld\n", mysql_affected_rows(db));

  return 0;

err:
  fprintf(stderr, "db_do_query failed: %s [%s]", mysql_error(db), query);
  return 1;
}
