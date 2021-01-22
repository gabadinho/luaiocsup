-- print("Loading "..arg[0])

function write_stringout(rec)
    date_format = nil
    -- date_format = arg[1]

    if (date_format==nil) then
        rec.update_field("VAL", os.date())
    else
        rec.update_field("VAL", os.date(date_format))
    end
end

