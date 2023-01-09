//
//  CorpusEntry.swift
//  Trad App
//
//  Created by damien murtagh on 1/8/23.
//

import Foundation

struct CorpusEntry: Codable, CustomStringConvertible
{
    var tuneReferenceNumber: Int
    var title: String
    var alt_title: String?
    var source: String
    var sourceid: Int
    var tradapp_id: String
    var key_sig: String
    var type: String
    var notation: String
    
    var description: String {
        return "Title: \(title)\n"  +
        (alt_title != nil ? "Alt Title: \(alt_title!)\n" : "") +
        "Source: \(source)\n" +
        "Sourceid: \(sourceid)\n" +
        "Tradapp_id: \(tradapp_id)\n" +
        "Key_sig: \(key_sig)\n" +
        "Type: \(type)\n" +
        "Notation: \(notation)\n"
    }
}
