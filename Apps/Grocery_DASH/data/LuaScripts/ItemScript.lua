function determineShelf(shelfNum, itemNum, one, two, three, four, five, six, seven, eight, nine, ten, eleven, twelve)
	
	local price = -18.00
	
	if 			shelfNum == one 	then	price = lookupItemInfoShelf1(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == two 	then	price = lookupItemInfoShelf2(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == three 	then	price = lookupItemInfoShelf3(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == four 	then	price = lookupItemInfoShelf4(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == five 	then	price = lookupItemInfoShelf5(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == six 	then	price = lookupItemInfoShelf6(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == seven 	then	price = lookupItemInfoShelf7(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == eight 	then	price = lookupItemInfoShelf8(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == nine 	then	price = lookupItemInfoShelf9(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == ten 	then	price = lookupItemInfoShelf10(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == eleven 	then	price = lookupItemInfoShelf11(itemNum, one, two, three, four, five, six, seven, eight)
	elseif 		shelfNum == twelve 	then	price = lookupItemInfoShelf12(itemNum, one, two, three, four, five, six, seven, eight)
	else									price = randomPrice()
	end

	return price
	
end

function lookupItemInfoShelf1(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (1.50)
	elseif 		itemNum == 2 	then	return (4.99)
	elseif 		itemNum == 3 	then	return (1.00)
	elseif 		itemNum == 4 	then	return (0.59)
	elseif 		itemNum == 5 	then	return (2.99)
	elseif 		itemNum == 6 	then	return (1.99)
	elseif 		itemNum == 7 	then	return (1.99)
	elseif 		itemNum == 8 	then	return (3.99)
	else								return randomPrice()
	end
	
end


function lookupItemInfoShelf2(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (2.79)
	elseif 		itemNum == 2 	then	return (1.50)
	elseif 		itemNum == 3 	then	return (5.99)
	elseif 		itemNum == 4 	then	return (3.59)
	elseif 		itemNum == 5 	then	return (2.99)
	elseif 		itemNum == 6 	then	return (1.99)
	elseif 		itemNum == 7 	then	return (2.99)
	elseif 		itemNum == 8 	then	return (4.29)
	else								return randomPrice()
	end

end


function lookupItemInfoShelf3(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (3.99)
	elseif 		itemNum == 2 	then	return (2.79)
	elseif 		itemNum == 3 	then	return (1.99)
	elseif 		itemNum == 4 	then	return (2.99)
	elseif 		itemNum == 5 	then	return (3.79)
	elseif 		itemNum == 6 	then	return (2.99)
	elseif 		itemNum == 7 	then	return (2.99)
	elseif 		itemNum == 8 	then	return (3.79)
	else								return randomPrice()
	end
	
end


function lookupItemInfoShelf4(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (1.99)
	elseif 		itemNum == 2 	then	return (1.99)
	elseif 		itemNum == 3 	then	return (2.59)
	elseif 		itemNum == 4 	then	return (2.99)
	elseif 		itemNum == 5 	then	return (3.99)
	elseif 		itemNum == 6 	then	return (2.99)
	elseif 		itemNum == 7 	then	return (2.99)
	elseif 		itemNum == 8 	then	return (4.29)
	else								return randomPrice()
	end
	
end


function lookupItemInfoShelf5(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (1.99)
	elseif 		itemNum == 2 	then	return (1.99)
	elseif 		itemNum == 3 	then	return (1.99)
	elseif 		itemNum == 4 	then	return (1.99)
	elseif 		itemNum == 5 	then	return (3.99)
	elseif 		itemNum == 6 	then	return (4.29)
	elseif 		itemNum == 7 	then	return (4.29)
	elseif 		itemNum == 8 	then	return (4.29)
	else								return randomPrice()
	end

end


function lookupItemInfoShelf6(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (0.65)
	elseif 		itemNum == 2 	then	return (0.89)
	elseif 		itemNum == 3 	then	return (1.00)
	elseif 		itemNum == 4 	then	return (6.50)
	elseif 		itemNum == 5 	then	return (5.79)
	elseif 		itemNum == 6 	then	return (2.99)
	elseif 		itemNum == 7 	then	return (6.99)
	elseif 		itemNum == 8 	then	return (1.99)
	else								return randomPrice()
	end

end


function lookupItemInfoShelf7(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (1.00)
	elseif 		itemNum == 2 	then	return (1.99)
	elseif 		itemNum == 3 	then	return (0.99)
	elseif 		itemNum == 4 	then	return (4.29)
	elseif 		itemNum == 5 	then	return (1.29)
	elseif 		itemNum == 6 	then	return (3.79)
	elseif 		itemNum == 7 	then	return (3.25)
	elseif 		itemNum == 8 	then	return (3.79)
	else								return randomPrice()
	end

end


function lookupItemInfoShelf8(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (2.99)
	elseif 		itemNum == 2 	then	return (2.79)
	elseif 		itemNum == 3 	then	return (1.99)
	elseif 		itemNum == 4 	then	return (2.99)
	elseif 		itemNum == 5 	then	return (1.99)
	elseif 		itemNum == 6 	then	return (1.99)
	elseif 		itemNum == 7 	then	return (2.99)
	elseif 		itemNum == 8 	then	return (3.99)
	else								return randomPrice()
	end

end

function lookupItemInfoShelf9(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (3.99)
	elseif 		itemNum == 2 	then	return (9.99)
	elseif 		itemNum == 3 	then	return (3.99)
	elseif 		itemNum == 4 	then	return (1.49)
	elseif 		itemNum == 5 	then	return (0.99)
	elseif 		itemNum == 6 	then	return (1.29)
	elseif 		itemNum == 7 	then	return (1.79)
	elseif 		itemNum == 8 	then	return (3.99)
	else								return randomPrice()
	end

end

function lookupItemInfoShelf10(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == 1 	then	return (3.99)
	elseif 		itemNum == 2 	then	return (2.79)
	elseif 		itemNum == 3 	then	return (2.99)
	elseif 		itemNum == 4 	then	return (1.79)
	elseif 		itemNum == 5 	then	return (4.99)
	elseif 		itemNum == 6 	then	return (2.99)
	elseif 		itemNum == 7 	then	return (1.79)
	elseif 		itemNum == 8 	then	return (1.99)
	else								return randomPrice()
	end

end

function lookupItemInfoShelf11(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == one 		then	return (2.79)
	elseif 		itemNum == two 		then	return (0.79)
	elseif 		itemNum == three 	then	return (1.00)
	elseif 		itemNum == four 	then	return (0.80)
	elseif 		itemNum == five 	then	return (1.00)
	elseif 		itemNum == six 		then	return (2.99)
	elseif 		itemNum == seven 	then	return (1.29)
	elseif 		itemNum == eight 	then	return (2.99)
	else									return randomPrice()
	end

end

function lookupItemInfoShelf12(itemNum, one, two, three, four, five, six, seven, eight)

	if 			itemNum == one 		then	return (4.99)
	elseif 		itemNum == two 		then	return (1.99)
	elseif 		itemNum == three 	then	return (2.99)
	elseif 		itemNum == four 	then	return (2.79)
	elseif 		itemNum == five 	then	return (0.80)
	elseif 		itemNum == six 		then	return (1.99)
	elseif 		itemNum == seven 	then	return (1.79)
	elseif 		itemNum == eight 	then	return (3.99)
	else									return randomPrice()
	end

end


function randomPrice()
	return 7.00
end