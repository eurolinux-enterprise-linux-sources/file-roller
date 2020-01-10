/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 *  File-Roller
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FR_WINDOW_H
#define FR_WINDOW_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include "typedefs.h"
#include "fr-archive.h"
#include "fr-error.h"


enum {
	COLUMN_FILE_DATA,
	COLUMN_ICON,
	COLUMN_NAME,
	COLUMN_EMBLEM,
	COLUMN_SIZE,
	COLUMN_TYPE,
	COLUMN_TIME,
	COLUMN_PATH,
	NUMBER_OF_COLUMNS
};

enum {
	TREE_COLUMN_PATH,
	TREE_COLUMN_ICON,
	TREE_COLUMN_NAME,
	TREE_COLUMN_WEIGHT,
	TREE_NUMBER_OF_COLUMNS
};

typedef enum {
	FR_BATCH_ACTION_NONE,
	FR_BATCH_ACTION_LOAD,
	FR_BATCH_ACTION_OPEN,
	FR_BATCH_ACTION_ADD,
	FR_BATCH_ACTION_EXTRACT,
	FR_BATCH_ACTION_EXTRACT_HERE,
	FR_BATCH_ACTION_EXTRACT_INTERACT,
	FR_BATCH_ACTION_RENAME,
	FR_BATCH_ACTION_PASTE,
	FR_BATCH_ACTION_OPEN_FILES,
	FR_BATCH_ACTION_SAVE_AS,
	FR_BATCH_ACTION_TEST,
	FR_BATCH_ACTION_ENCRYPT,
	FR_BATCH_ACTION_CLOSE,
	FR_BATCH_ACTION_QUIT,
	FR_BATCH_ACTIONS
} FrBatchActionType;

/* -- FrWindow -- */

#define FR_TYPE_WINDOW              (fr_window_get_type ())
#define FR_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), FR_TYPE_WINDOW, FrWindow))
#define FR_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), FR_WINDOW_TYPE, FrWindowClass))
#define FR_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FR_TYPE_WINDOW))
#define FR_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), FR_TYPE_WINDOW))
#define FR_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), FR_TYPE_WINDOW, FrWindowClass))

typedef struct _FrWindow        FrWindow;
typedef struct _FrWindowClass   FrWindowClass;
typedef struct _FrWindowPrivate FrWindowPrivate;

struct _FrWindow
{
	GtkApplicationWindow __parent;
	FrWindowPrivate *priv;
	FrArchive *archive;
};

struct _FrWindowClass
{
	GtkApplicationWindowClass __parent_class;

	/*<signals>*/

	void (*archive_loaded) (FrWindow   *window,
				gboolean    success);
	void (*progress)       (FrWindow   *window,
			        double      fraction,
			        const char *msg);
	void (*ready)          (FrWindow   *window,
				GError     *error);
};

GType           fr_window_get_type                     (void);
GtkWidget *     fr_window_new                          (void);
void            fr_window_close                        (FrWindow      *window);

/* archive operations */

gboolean        fr_window_archive_new                  (FrWindow      *window,
							GFile         *file,
						        const char    *mime_type);
FrWindow *      fr_window_archive_open                 (FrWindow      *window,
							GFile         *file,
						        GtkWindow     *parent);
void            fr_window_archive_close                (FrWindow      *window);
GFile *         fr_window_get_archive_file             (FrWindow      *window);
GFile *         fr_window_get_archive_file_for_paste   (FrWindow      *window);
gboolean        fr_window_archive_is_present           (FrWindow      *window);
void            fr_window_archive_reload               (FrWindow      *window);
void            fr_window_archive_add_files            (FrWindow      *window,
							GList         *file_list, /* GFile list */
							GFile         *base_dir,
							gboolean       update);
void            fr_window_archive_add_with_filter      (FrWindow      *window,
							GList         *files, /* GFile list */
							GFile         *base_dir,
						        const char    *include_files,
						        const char    *exclude_files,
						        const char    *exclude_folders,
						        const char    *dest_dir,
						        gboolean       update,
						        gboolean       follow_links);
void            fr_window_archive_add_dropped_items    (FrWindow      *window,
						        GList         *file_list);
void            fr_window_archive_remove               (FrWindow      *window,
						        GList         *file_list);
void            fr_window_archive_extract              (FrWindow      *window,
						        GList         *file_list,
						        GFile         *destination,
						        const char    *base_dir,
						        gboolean       skip_older,
						        FrOverwrite    overwrite,
						        gboolean       junk_paths,
						        gboolean       ask_to_open_destination);
void            fr_window_archive_extract_here         (FrWindow      *window,
						        gboolean       skip_older,
						        gboolean       overwrite,
						        gboolean       junk_paths,
							gboolean       ask_to_open_destination);
void            fr_window_archive_test	               (FrWindow      *window);
void            fr_window_archive_encrypt              (FrWindow      *window,
							const char    *password,
							gboolean       encrypt_header);

/**/

void            fr_window_set_password                 (FrWindow      *window,
						        const char    *password);
void            fr_window_set_password_for_second_archive
						       (FrWindow      *window,
						        const char    *password);
const char *    fr_window_get_password                 (FrWindow      *window);
const char *    fr_window_get_password_for_second_archive
						       (FrWindow      *window);
void            fr_window_set_encrypt_header           (FrWindow      *window,
						        gboolean       encrypt_header);
gboolean        fr_window_get_encrypt_header           (FrWindow      *window);
void            fr_window_set_compression 	       (FrWindow      *window,
						        FrCompression  compression);
FrCompression   fr_window_get_compression 	       (FrWindow      *window);
void            fr_window_set_volume_size 	       (FrWindow      *window,
						        guint          volume_size);
guint           fr_window_get_volume_size 	       (FrWindow      *window);

/**/

void            fr_window_go_to_location               (FrWindow      *window,
						        const char    *path,
						        gboolean       force_update);
const char *    fr_window_get_current_location         (FrWindow      *window);
void            fr_window_go_up_one_level              (FrWindow      *window);
void            fr_window_go_back                      (FrWindow      *window);
void            fr_window_go_forward                   (FrWindow      *window);
void            fr_window_current_folder_activated     (FrWindow      *window,
						        gboolean       from_sidebar);
void            fr_window_set_list_mode                (FrWindow      *window,
						        FrWindowListMode
						        	       list_mode);

/**/

GList *         fr_window_get_file_list_selection      (FrWindow    *window,
						        gboolean     recursive,
						        gboolean    *has_dirs);
GList *         fr_window_get_file_list_from_path_list (FrWindow    *window,
						        GList       *path_list,
						        gboolean    *has_dirs);
GList *         fr_window_get_file_list_pattern        (FrWindow    *window,
						        const char  *pattern);
int             fr_window_get_n_selected_files         (FrWindow    *window);
GList *         fr_window_get_folder_tree_selection    (FrWindow    *window,
						        gboolean     recursive,
						        gboolean    *has_dirs);
GList *         fr_window_get_selection                (FrWindow    *window,
						        gboolean     from_sidebar,
						        char       **return_base_dir);
GtkTreeModel *  fr_window_get_list_store               (FrWindow    *window);
void            fr_window_find                         (FrWindow    *window,
							gboolean     active);
void            fr_window_select_all                   (FrWindow    *window);
void            fr_window_unselect_all                 (FrWindow    *window);

/**/

void            fr_window_rename_selection             (FrWindow    *window,
						        gboolean     from_sidebar);
void            fr_window_cut_selection                (FrWindow    *window,
						        gboolean     from_sidebar);
void            fr_window_copy_selection               (FrWindow    *window,
						        gboolean     from_sidebar);
void            fr_window_paste_selection              (FrWindow    *window,
						        gboolean     from_sidebar);

/**/

void            fr_window_stop                         (FrWindow    *window);

/**/

void            fr_window_action_new_archive           (FrWindow   *window);
gboolean        fr_window_create_archive_and_continue  (FrWindow   *window,
							GFile      *file,
							const char *mime_type,
							GtkWindow  *error_dialog_parent);
void            fr_window_action_save_as               (FrWindow   *window);
void            fr_window_view_last_output             (FrWindow   *window,
						        const char *title);
void            fr_window_open_files                   (FrWindow   *window,
						        GList      *file_list,
						        gboolean    ask_application);
void            fr_window_open_files_with_command      (FrWindow   *window,
						        GList      *file_list,
						        char       *command);
void            fr_window_open_files_with_application  (FrWindow   *window,
						        GList      *file_list,
						        GAppInfo   *app);
gboolean        fr_window_update_files                 (FrWindow   *window,
						        GList      *file_list);
void            fr_window_update_history_list          (FrWindow   *window);
void            fr_window_set_default_dir              (FrWindow   *window,
							GFile      *default_dir,
						        gboolean    freeze);
void            fr_window_set_open_default_dir         (FrWindow   *window,
							GFile      *default_dir);
GFile *         fr_window_get_open_default_dir         (FrWindow   *window);
void            fr_window_set_add_default_dir          (FrWindow   *window,
							GFile      *default_dir);
GFile *         fr_window_get_add_default_dir          (FrWindow   *window);
void            fr_window_set_extract_default_dir      (FrWindow   *window,
							GFile      *default_dir,
						        gboolean    freeze);
GFile *         fr_window_get_extract_default_dir      (FrWindow   *window);
void            fr_window_set_statusbar_visibility     (FrWindow   *window,
						        gboolean    value);
void            fr_window_set_folders_visibility       (FrWindow   *window,
						        gboolean    value);
void            fr_window_use_progress_dialog          (FrWindow   *window,
						        gboolean    value);

/* batch mode procedures. */

void            fr_window_new_batch                    (FrWindow      *window,
						        const char    *title);
const char *    fr_window_get_batch_title              (FrWindow      *window);
void            fr_window_set_current_batch_action     (FrWindow      *window,
						        FrBatchActionType
						        	       action,
						        void          *data,
						        GFreeFunc      free_func);
void            fr_window_reset_current_batch_action   (FrWindow      *window);
void            fr_window_restart_current_batch_action (FrWindow      *window);
void            fr_window_append_batch_action          (FrWindow      *window,
						        FrBatchActionType
						        	       action,
						        void          *data,
						        GFreeFunc      free_func);
void            fr_window_start_batch                  (FrWindow      *window);
void            fr_window_stop_batch                   (FrWindow      *window);
void            fr_window_stop_batch_with_error        (FrWindow     *window,
							FrAction      action,
							FrErrorType   error_type,
							const char   *error_message);
void            fr_window_resume_batch                 (FrWindow      *window);
gboolean        fr_window_is_batch_mode                (FrWindow      *window);
void            fr_window_set_batch__extract           (FrWindow      *window,
						        GFile         *archive,
						        GFile         *destination);
void            fr_window_set_batch__extract_here      (FrWindow      *window,
							GFile         *archive);
void            fr_window_set_batch__add               (FrWindow      *window,
							GFile         *archive,
						        GList         *file_list);
void            fr_window_destroy_with_error_dialog    (FrWindow      *window);
void            fr_window_set_notify                   (FrWindow      *window,
							gboolean       notify);

/**/

gboolean        fr_window_file_list_drag_data_get      (FrWindow         *window,
						        GdkDragContext   *context,
						        GtkSelectionData *selection_data,
						        GList            *path_list);
void            fr_window_update_dialog_closed         (FrWindow         *window);

#endif /* FR_WINDOW_H */
