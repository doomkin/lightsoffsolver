/*
 * progress.h
 *
 * Copyright (C) 2017 - Pavel Nikitin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROGRESS_H_
#define PROGRESS_H_

#define PROGRESS_MESSAGE_WIDTH 30
#define PROGRESS_BAR_LENGTH    50

/**
 * SECTION: progress
 * @title: progress
 * @short_description: Shows progress bar into console.
 */

/**
 * show_progress:
 * @message: Message about progress
 * @percent: Percent of progress
 *
 * Shows progress bar into console.
 **/
void
show_progress (const char *message,
               int         percent);

#endif
